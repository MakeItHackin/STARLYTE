
#  STARLYTE WEATHER LOCATION COMPARISON
#  SUBMITTED FOR 2017 NASA INTERNATIONAL SPACE APPS CHALLENGE
#  PROJECT: STARLYTE
#  AUTHORS: TILANI NEADEAU, ANDREW DENIO
#  DATE: 5/5/2017

import urllib.request as request
import json

#PUT YOUR WEATHER UNDERGROUND API KEY BETWEEN THE QUOTES BELOW:
key = "" 

#IUEYETUO2 IS A WEATHER STATION IN UTC TIME.  ALL WE ARE GRABBING HERE IS THE UTC TIME
fileName = "http://api.wunderground.com/api/" + key +    "/alerts/astronomy/hourly/geolookup/tide/conditions/forecast/q/pws:IUEYETUO2.json"


#PARSE TIME FROM JSON REQUEST
f = request.urlopen(fileName)
json_string = f.read().decode('utf-8')
parsed_json = json.loads(json_string)
local_time = parsed_json['current_observation']['local_time_rfc822']
obs_time = parsed_json['current_observation']['observation_time_rfc822']

#PRINT OUT THE TIMES
print ("%s@%s" % (local_time, obs_time))


