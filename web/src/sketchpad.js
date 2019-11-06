class Sketchpad {
  constructor(options) {
    // Support both old api (element) and new (canvas)
    options.canvas = options.canvas || options.element;
    if (!options.canvas) {
      console.error('[SKETCHPAD]: Please provide an element/canvas:');
        return;
    }

    if (typeof options.canvas === 'string') {
      options.canvas = document.querySelector(options.canvas);
    }
    this.canvas = options.canvas;

    if (typeof options.destination === 'string') {
        options.destination = document.querySelector(options.destination);
    } else this.destination = document.createElement('canvas');

    this.destination = options.destination;

    // Try to extract 'width', 'height', 'color', 'penSize' and 'readOnly'
    // from the options or the DOM element.
    ['width', 'height', 'color', 'penSize', 'readOnly'].forEach(function(attr) {
      this[attr] = options[attr] || this.canvas.getAttribute('data-' + attr);
    }, this);

    // Setting default values
    this.width = this.width || 0;
    this.height = this.height || 0;

    this.color = this.color || '#000';
    this.penSize = this.penSize || 5;

    this.readOnly = this.readOnly || false;

    // Sketchpad History settings
    this.strokes = options.strokes || [];

    this.undoHistory = options.undoHistory || [];

    // Enforce context for Moving Callbacks
    this.onMouseMove = this.onMouseMove.bind(this);

    // Setup Internal Events
    this.events = {};
    this.events['mousemove'] = [];
    this.events['animation-end'] = [];
    this.events['show'] = [];
    this.internalEvents = ['MouseDown', 'MouseUp', 'MouseOut'];
    this.internalEvents.forEach(function(name) {
      let lower = name.toLowerCase();
      this.events[lower] = [];

      // Enforce context for Internal Event Functions
      this['on' + name] = this['on' + name].bind(this);

      // Add DOM Event Listeners
      this.canvas.addEventListener(lower, (...args) => this.trigger(lower, args));
    }, this);

    this.reset();
  }

  /*
   * Private API
   */

  _position(event) {
      var rect = this.canvas.getBoundingClientRect();
      var scrollLeft = window.pageXOffset || document.documentElement.scrollLeft;
      var scrollTop = window.pageYOffset || document.documentElement.scrollTop;
    return {
        x: event.pageX - rect.left + scrollLeft,
        y: event.pageY - rect.top - scrollTop,
    };
  }

  _stroke(stroke) {
    if (stroke.type === 'clear') {
        this.drawBlack();
    } else {
        stroke.lines.forEach(function (line) {
            this._line(line.start, line.end, stroke.color, stroke.size);
        }, this);
    }
  }

  _draw(start, end, color, size) {
    this._line(start, end, color, size, 'source-over');
    this.resize();
  }

  _erase(start, end, color, size) {
    this._line(start, end, color, size, 'destination-out');
  }

  _line(start, end, color, size, compositeOperation) {
    this.context.save();
    this.context.lineJoin = 'round';
    this.context.lineCap = 'round';
    this.context.strokeStyle = color;
    this.context.lineWidth = size;
    this.context.globalCompositeOperation = compositeOperation;
    this.context.beginPath();
    this.context.moveTo(start.x, start.y);
    this.context.lineTo(end.x, end.y);
    this.context.closePath();
    this.context.stroke();
    this.context.restore();
  }

  /*
   * Events/Callback
   */

  onMouseDown(event) {
    this._sketching = true;
    this._lastPosition = this._position(event);
    this._currentStroke = {
      color: this.color,
      size: this.penSize,
      lines: [],
    };

    this.canvas.addEventListener('mousemove', this.onMouseMove);
  }

  onMouseUp() {
    if (this._sketching) {
      this.strokes.push(this._currentStroke);
      this._sketching = false;
    }

    this.canvas.removeEventListener('mousemove', this.onMouseMove);
  }

  onMouseOut(event) {
    this.onMouseUp(event);
  }

  onMouseMove(event) {
    let currentPosition = this._position(event);
    this._draw(this._lastPosition, currentPosition, this.color, this.penSize);
    this._currentStroke.lines.push({
      start: this._lastPosition,
      end: currentPosition,
    });
    this._lastPosition = currentPosition;

    this.trigger('mousemove', [event]);
  }

  /*
   * Public API
   */

  toObject() {
    return {
      width: this.canvas.width,
      height: this.canvas.height,
      strokes: this.strokes,
      undoHistory: this.undoHistory,
    };
  }

  toJSON() {
    return JSON.stringify(this.toObject());
  }

  resize() {
      //resize to hektopixel
      // step 1
      const oc = document.createElement('canvas');
      const octx = oc.getContext('2d');
      oc.width = this.width;
      oc.height = this.height;

      // steo 2: pre-filter image using steps as radius
      const steps = (oc.width / 50)>>1;
      octx.filter = `blur(${steps}px)`;
      octx.drawImage(this.canvas, 0, 0);

      this.destinationContext.drawImage(oc, 0, 0, 20, 15);

      let imageData = this.destinationContext.getImageData(0,0,20,15);
      let rgbArray = [];
      //map pixels to rgb array
      for (var i = 0; i < imageData.data.length; i+=4) {
          rgbArray.push([imageData.data[i], imageData.data[i+1], imageData.data[i+2]]);
      }
      let snakewiseArray = [];
      for(let x = 0; x < 15; x++) {
          for(let y = 0; y < 20; y++) {
              let px = (x%2===0?(x-x%2) * 20  + y:(x+1) * 20  - y - 1);
              snakewiseArray.push(rgbArray[px][0]);
              snakewiseArray.push(rgbArray[px][1]);
              snakewiseArray.push(rgbArray[px][2]);
          }
      }
      let bytestream = Uint8Array.from(snakewiseArray);
      this.trigger('show', [bytestream]);
  }

  redo() {
    var stroke = this.undoHistory.pop();
    if (stroke) {
      this.strokes.push(stroke);
      this._stroke(stroke);
    }
    this.resize();
  }

  undo() {
    this.drawBlack();
    var stroke = this.strokes.pop();
    this.redraw();

    if (stroke) {
      this.undoHistory.push(stroke);
    }
  }

  clear() {
    this.strokes.push({
      type: 'clear',
    });
      this.drawBlack();
  }

  redraw() {
    this.strokes.forEach(function(stroke) {
      this._stroke(stroke);
    }, this);
    this.resize();
  }

  reset() {
    // Setup canvas
    this.canvas.width = this.width;
    this.canvas.height = this.height;
    this.context = this.canvas.getContext('2d');

    this.destinationContext = this.destination.getContext('2d');
    this.destination.width  = 20;
    this.destination.height = 15;

    // Redraw image
    this.drawBlack();
    this.cancelAnimation();
    this.redraw();

    // Remove all event listeners, this way readOnly option will be respected
    // on the reset
    this.internalEvents.forEach(name => this.off(name.toLowerCase()));

    if (this.readOnly) {
      return;
    }

    // Re-Attach all event listeners
    this.internalEvents.forEach(name => this.on(name.toLowerCase(), this['on' + name]));
  }

  cancelAnimation() {
    this.animateIds = this.animateIds || [];
    this.animateIds.forEach(function(id) {
      clearTimeout(id);
    });
    this.animateIds = [];
  }

  drawBlack() {
    //fill black
    this.context.beginPath();
    this.context.rect(0, 0, this.width, this.height);
    this.context.fillStyle = "black";
    this.context.fill();
  }

  animate(interval=10, loop=false, loopInterval=0) {
    let delay = interval;

    this.cancelAnimation();
    this.drawBlack();

    this.strokes.forEach(stroke => {
      if (stroke.type === 'clear') {
        delay += interval;
        return this.animateIds.push(setTimeout(() => {
            this.drawBlack();
        }, delay));
      }

      stroke.lines.forEach(line => {
        delay += interval;
        this.animateIds.push(setTimeout(() => {
          this._draw(line.start, line.end, stroke.color, stroke.size);
        }, delay));
      });
    });

    if (loop) {
      this.animateIds.push(setTimeout(() => {
        this.animate(interval=10, loop, loopInterval);
      }, delay + interval + loopInterval));
    }

    this.animateIds.push(setTimeout(() => {
      this.trigger('animation-end', [interval, loop, loopInterval]);
    }, delay + interval));
  }

  /*
   * Event System
   */

  /* Attach an event callback
   *
   * @param {String} action Which action will have a callback attached
   * @param {Function} callback What will be executed when this event happen
   */
  on(action, callback) {
    // Tell the user if the action he has input was invalid
    if (this.events[action] === undefined) {
      return console.error(`Sketchpad: No such action '${action}'`);
    }

    this.events[action].push(callback);
  }

  /* Detach an event callback
   *
   * @param {String} action Which action will have event(s) detached
   * @param {Function} callback Which function will be detached. If none is
   *                            provided, all callbacks are detached
   */
  off(action, callback) {
    if (callback) {
      // If a callback has been specified delete it specifically
      var index = this.events[action].indexOf(callback);
      (index !== -1) && this.events[action].splice(index, 1);
      return index !== -1;
    }

    // Else just erase all callbacks
    this.events[action] = [];
  }

  /* Trigger an event
   *
   * @param {String} action Which event will be triggered
   * @param {Array} args Which arguments will be provided to the callbacks
   */
  trigger(action, args=[]) {
    // Fire all events with the given callback
    this.events[action].forEach(function(callback) {
      callback(...args);
    });
  }
}

window.Sketchpad = Sketchpad;
export default Sketchpad;