var ical = require('./ical');

var url = '';

var download = function (url, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {                                                                                                                                                   
    callback(null, this.responseText);
  };
  xhr.open('GET', url);
  xhr.send();
};

var findClosest = function (data) {
  var now = new Date();
  var closest = null;
  
  for (var k in data) {
    if (data.hasOwnProperty(k) && data[k].type === 'VEVENT') {
      var ev = data[k];
      console.log('Event : ' + JSON.stringify(ev));
      
      if (now > ev.start) {
        console.log('Fail for ' + ev.summary + ' because ' + ev.start);
        continue;
      }
      
      if (closest === null) {
        closest = ev;
      } else if (ev.start < closest.start) {
        closest = ev;
      }
    }
  }
  
  return closest;
};

var format = function (event) {
  return {
    SUMMARY: event.summary,
    LOCATION: event.location,
    DATE: event.start.toDateString() + ' ' + 
      event.start.getHours() + 'h' +
      event.start.getMinutes(),
  };
};



var sendICal = function () {
  console.log('Looking for ' + url);
  download(url, function (err, data) {
    if (err) {
      return;
    }    
    var ics = ical.parseICS(data);   
    console.log('Found data ' + JSON.stringify(ics));
    var closest = findClosest (ics);
    
    Pebble.sendAppMessage(format(closest), function () {
      console.log('Sent ' + JSON.stringify(closest));
    }, function () {
      console.log('Fail !');
    });
  });
};

// Called when JS is ready
Pebble.addEventListener("ready", function(e) {
  console.log("JS is ready!");
  sendICal();
});
												
// Called when incoming message from the Pebble is received
// We are currently only checking the "message" appKey defined in appinfo.json/Settings
Pebble.addEventListener("appmessage", function(e) {
  console.log("Received Message: " + e.payload.message);
});