var ical = require('ical');

var url = '';

var findClosest = function (data) {
  var now = new Date();
  var closest = null;
  
  for (var k in data) {
    if (data.hasOwnProperty(k)) {
      var ev = data[k];
      
      if (now > ev.start.getDate()) {
        continue;
      }
      
      if (closest === null) {
        closest = ev;
      } else if (ev.start.getDate() < closest.start.getDate()) {
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
    DATE: event.start.getDate().toDateString(),
  };
};

var sendICal = function () {
  console.log('Looking for ' + url);
  ical.fromURL(url, {}, function (err, data) {
    if (err) {
      console.log(err);
    } 
    console.log('Found data');
    var closest = findClosest (data);
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