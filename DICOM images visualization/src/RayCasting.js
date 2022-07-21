import '@kitware/vtk.js/favicon';
import { useState, useRef, useEffect } from 'react';

import { vec3, quat, mat4 } from 'gl-matrix';

// Load the rendering pieces we want to use (for both WebGL and WebGPU)
import '@kitware/vtk.js/Rendering/Profiles/Geometry';
import '@kitware/vtk.js/Rendering/Profiles/Volume';
import '@kitware/vtk.js/Rendering/Profiles/Glyph';

// Force DataAccessHelper to have access to various data source
import '@kitware/vtk.js/IO/Core/DataAccessHelper/HtmlDataAccessHelper';
import '@kitware/vtk.js/IO/Core/DataAccessHelper/HttpDataAccessHelper';
import '@kitware/vtk.js/IO/Core/DataAccessHelper/JSZipDataAccessHelper';

import vtkActor from '@kitware/vtk.js/Rendering/Core/Actor';
import vtkFullScreenRenderWindow from '@kitware/vtk.js/Rendering/Misc/FullScreenRenderWindow';
import vtkHttpDataSetReader from '@kitware/vtk.js/IO/Core/HttpDataSetReader';
import vtkImageMarchingCubes from '@kitware/vtk.js/Filters/General/ImageMarchingCubes';
import vtkMapper from '@kitware/vtk.js/Rendering/Core/Mapper';
import vtkWidgetManager from '@kitware/vtk.js/Widgets/Core/WidgetManager';
import vtkImageCroppingWidget from '@kitware/vtk.js/Widgets/Widgets3D/ImageCroppingWidget';
import vtkColorTransferFunction from '@kitware/vtk.js/Rendering/Core/ColorTransferFunction';
import vtkColorMaps from '@kitware/vtk.js/Rendering/Core/ColorTransferFunction/ColorMaps';
import vtkPiecewiseFunction from '@kitware/vtk.js/Common/DataModel/PiecewiseFunction';
import vtkPiecewiseGaussianWidget from '@kitware/vtk.js/Interaction/Widgets/PiecewiseGaussianWidget';
import vtkVolume from '@kitware/vtk.js/Rendering/Core/Volume';
import vtkVolumeMapper from '@kitware/vtk.js/Rendering/Core/VolumeMapper';
import vtkPlane from '@kitware/vtk.js/Common/DataModel/Plane';

let RayCasting = ({ link }) => {
  const rootContainer = document.querySelector(
    '.vtk-js-example-piecewise-gaussian-widget'
  );
  const containerStyle = rootContainer ? { height: '100%' } : null;
  const urlToLoad = link;
  const vtkContainerRef = useRef(null);

  useEffect(() => {
    const fullScreenRenderer = vtkFullScreenRenderWindow.newInstance({
      rootContainer: vtkContainerRef.current,
    });
    const renderer = fullScreenRenderer.getRenderer();
    const renderWindow = fullScreenRenderer.getRenderWindow();

    renderWindow.getInteractor().setDesiredUpdateRate(15.0);

    const body = rootContainer || document.querySelector('body');

    const widgetContainer = document.createElement('div');
    widgetContainer.style.position = 'absolute';
    widgetContainer.style.top = 'calc(10px + 3.5em)';
    widgetContainer.style.left = '5px';
    widgetContainer.style.background = 'rgba(255, 255, 255, 0.3)';
    body.appendChild(widgetContainer);

    const labelContainer = document.createElement('div');
    labelContainer.style.position = 'absolute';
    labelContainer.style.top = '40px';
    labelContainer.style.left = '35px';
    labelContainer.style.width = '100%';
    labelContainer.style.color = 'white';
    labelContainer.style.userSelect = 'none';
    labelContainer.style.cursor = 'pointer';
    body.appendChild(labelContainer);

    let presetIndex = 1;
    const globalDataRange = [0, 255];
    const lookupTable = vtkColorTransferFunction.newInstance();

    function changePreset(delta = 1) {
      presetIndex =
        (presetIndex + delta + vtkColorMaps.rgbPresetNames.length) %
        vtkColorMaps.rgbPresetNames.length;
      lookupTable.applyColorMap(
        vtkColorMaps.getPresetByName(vtkColorMaps.rgbPresetNames[presetIndex])
      );
      lookupTable.setMappingRange(...globalDataRange);
      lookupTable.updateRange();
      labelContainer.innerHTML = vtkColorMaps.rgbPresetNames[presetIndex];
    }

    let intervalID = null;
    function stopInterval() {
      if (intervalID !== null) {
        clearInterval(intervalID);
        intervalID = null;
      }
    }

    labelContainer.addEventListener('click', (event) => {
      if (event.pageX < 200) {
        stopInterval();
        changePreset(-1);
      } else {
        stopInterval();
        changePreset(1);
      }
    });

    const widget = vtkPiecewiseGaussianWidget.newInstance({
      numberOfBins: 256,
      size: [400, 150],
    });
    widget.updateStyle({
      backgroundColor: 'rgba(255, 255, 255, 0.6)',
      histogramColor: 'rgba(100, 100, 100, 0.5)',
      strokeColor: 'rgb(0, 0, 0)',
      activeColor: 'rgb(255, 255, 255)',
      handleColor: 'rgb(50, 150, 50)',
      buttonDisableFillColor: 'rgba(255, 255, 255, 0.5)',
      buttonDisableStrokeColor: 'rgba(0, 0, 0, 0.5)',
      buttonStrokeColor: 'rgba(0, 0, 0, 1)',
      buttonFillColor: 'rgba(255, 255, 255, 1)',
      strokeWidth: 2,
      activeStrokeWidth: 3,
      buttonStrokeWidth: 1.5,
      handleWidth: 3,
      iconSize: 20,
      padding: 10,
    });

    fullScreenRenderer.setResizeCallback(({ width, height }) => {
      widget.setSize(Math.min(450, width - 10), 150);
    });

    const piecewiseFunction = vtkPiecewiseFunction.newInstance();

    const actor = vtkVolume.newInstance();
    const mapper = vtkVolumeMapper.newInstance({ sampleDistance: 1.1 });
    const reader = vtkHttpDataSetReader.newInstance({ fetchGzip: true });

    reader.setUrl(urlToLoad).then(() => {
      reader.loadData().then(() => {
        const imageData = reader.getOutputData();
        const dataArray = imageData.getPointData().getScalars();
        const dataRange = dataArray.getRange();
        globalDataRange[0] = dataRange[0];
        globalDataRange[1] = dataRange[1];

        changePreset();

        if (!rootContainer) {
          intervalID = setInterval(changePreset, 5000);
        }

        widget.setDataArray(dataArray.getData());
        widget.applyOpacity(piecewiseFunction);

        widget.setColorTransferFunction(lookupTable);
        lookupTable.onModified(() => {
          widget.render();
          renderWindow.render();
        });

        renderer.addVolume(actor);
        renderer.resetCamera();
        renderer.getActiveCamera().elevation(70);
        renderWindow.render();
      });
    });

    actor.setMapper(mapper);
    mapper.setInputConnection(reader.getOutputPort());

    actor.getProperty().setRGBTransferFunction(0, lookupTable);
    actor.getProperty().setScalarOpacity(0, piecewiseFunction);
    actor.getProperty().setInterpolationTypeToFastLinear();

    widget.addGaussian(0.425, 0.5, 0.2, 0.3, 0.2);
    widget.addGaussian(0.75, 1, 0.3, 0, 0);

    widget.setContainer(widgetContainer);
    widget.bindMouseListeners();

    widget.onAnimation((start) => {
      if (start) {
        renderWindow.getInteractor().requestAnimation(widget);
      } else {
        renderWindow.getInteractor().cancelAnimation(widget);
      }
    });

    widget.onOpacityChange(() => {
      widget.applyOpacity(piecewiseFunction);
      if (!renderWindow.getInteractor().isAnimating()) {
        renderWindow.render();
      }
    });
    return null;
  }, [vtkContainerRef, link]);

  return (
    <div>
      <div ref={vtkContainerRef} />
    </div>
  );
};
export default RayCasting;