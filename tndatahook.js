module['exports'] = function fetchData(hook) {
  var request = require('request');
  var responses;
  var filteredResponses = [];

  var options = {
    url: "http://www.tdot.tn.gov/opendata/api/data/RoadwayMessageSigns",
    method: "GET",
    headers: { 
      apikey: "30c790d2b7b84867975220283669cbd0"
    }
  };
  
  var callback = function(error, response, body) {
    responses = JSON.parse(body)._embedded.items;
    responses.forEach(function(response) {
      var message = response.properties.message;
      if (filteredResponses.indexOf(message) === -1) {
        filteredResponses.push(message);
      }
    });
    hook.res.end(JSON.stringify(filteredResponses));
  };

  request(options, callback);
};
