
#  STARLYTE WEATHER LOCATION COMPARISON
#  SUBMITTED FOR 2017 NASA INTERNATIONAL SPACE APPS CHALLENGE
#  PROJECT: STARLYTE
#  AUTHORS: TILANI NEADEAU, ANDREW DENIO
#  DATE: 5/5/2017

import urllib.request as request
import json

#PUT YOUR WEATHER UNDERGROUND API KEY BETWEEN THE QUOTES BELOW:
key = ""  

#KHIKAMUE8 IS THE CLOSEST STATION TO THE HI-SEAS LOCATION
fileName = "http://api.wunderground.com/api/" + key +    "/alerts/astronomy/hourly/geolookup/tide/conditions/forecast/q/pws:KHIKAMUE8.json"

#PARSE TIME FROM JSON REQUEST
f = request.urlopen(fileName)
json_string = f.read().decode('utf-8')
parsed_json = json.loads(json_string)
local_time = parsed_json['current_observation']['local_time_rfc822']
sunrise_h = parsed_json['moon_phase']['sunrise']['hour']
sunrise_m = parsed_json['moon_phase']['sunrise']['minute']
sunset_h = parsed_json['moon_phase']['sunset']['hour']
sunset_m = parsed_json['moon_phase']['sunset']['minute']
obs_city = parsed_json['current_observation']['observation_location']['city']
obs_lat = parsed_json['current_observation']['observation_location']['latitude']
obs_lon = parsed_json['current_observation']['observation_location']['longitude']
obs_ele = parsed_json['current_observation']['observation_location']['elevation']
obs_time = parsed_json['current_observation']['observation_time_rfc822']
station_id = parsed_json['current_observation']['station_id']
alerts = parsed_json['alerts']
temp_f = parsed_json['current_observation']['temp_f']
temp_c = parsed_json['current_observation']['temp_c']
weather_conditions = parsed_json['current_observation']['weather']
relative_humidity = parsed_json['current_observation']['relative_humidity']
wind_degrees = parsed_json['current_observation']['wind_degrees']
wind_mph = parsed_json['current_observation']['wind_mph']
pressure_mb = parsed_json['current_observation']['pressure_mb']
pressure_in = parsed_json['current_observation']['pressure_in']
feelslike_f = parsed_json['current_observation']['feelslike_f']
feelslike_c = parsed_json['current_observation']['feelslike_c']
precip_today_in = parsed_json['current_observation']['precip_today_in']
solarradiation = parsed_json['current_observation']['solarradiation']

# 0 is today
forecast_0_highf = parsed_json['forecast']['simpleforecast']['forecastday'][0]['high']['fahrenheit']
forecast_0_highc = parsed_json['forecast']['simpleforecast']['forecastday'][0]['high']['celsius']
forecast_0_lowf = parsed_json['forecast']['simpleforecast']['forecastday'][0]['low']['fahrenheit']
forecast_0_lowc = parsed_json['forecast']['simpleforecast']['forecastday'][0]['low']['celsius']
forecast_0_conditions = parsed_json['forecast']['simpleforecast']['forecastday'][0]['conditions']
forecast_0_pop = parsed_json['forecast']['simpleforecast']['forecastday'][0]['pop']
forecast_0_inchesrain = parsed_json['forecast']['simpleforecast']['forecastday'][0]['qpf_allday']['in']
forecast_0_windspeed = parsed_json['forecast']['simpleforecast']['forecastday'][0]['avewind']['mph']
forecast_0_winddegrees = parsed_json['forecast']['simpleforecast']['forecastday'][0]['avewind']['degrees']
forecast_0_avghumidity= parsed_json['forecast']['simpleforecast']['forecastday'][0]['avehumidity']

#1 is tomorrow
forecast_1_highf = parsed_json['forecast']['simpleforecast']['forecastday'][1]['high']['fahrenheit']
forecast_1_highc = parsed_json['forecast']['simpleforecast']['forecastday'][1]['high']['celsius']
forecast_1_lowf = parsed_json['forecast']['simpleforecast']['forecastday'][1]['low']['fahrenheit']
forecast_1_lowc = parsed_json['forecast']['simpleforecast']['forecastday'][1]['low']['celsius']
forecast_1_conditions = parsed_json['forecast']['simpleforecast']['forecastday'][1]['conditions']
forecast_1_pop = parsed_json['forecast']['simpleforecast']['forecastday'][1]['pop']
forecast_1_inchesrain = parsed_json['forecast']['simpleforecast']['forecastday'][1]['qpf_allday']['in']
forecast_1_windspeed = parsed_json['forecast']['simpleforecast']['forecastday'][1]['avewind']['mph']
forecast_1_winddegrees = parsed_json['forecast']['simpleforecast']['forecastday'][1]['avewind']['degrees']
forecast_1_avghumidity= parsed_json['forecast']['simpleforecast']['forecastday'][1]['avehumidity']

#2 is the day after tomorrow
forecast_2_highf = parsed_json['forecast']['simpleforecast']['forecastday'][2]['high']['fahrenheit']
forecast_2_highc = parsed_json['forecast']['simpleforecast']['forecastday'][2]['high']['celsius']
forecast_2_lowf = parsed_json['forecast']['simpleforecast']['forecastday'][2]['low']['fahrenheit']
forecast_2_lowc = parsed_json['forecast']['simpleforecast']['forecastday'][2]['low']['celsius']
forecast_2_conditions = parsed_json['forecast']['simpleforecast']['forecastday'][2]['conditions']
forecast_2_pop = parsed_json['forecast']['simpleforecast']['forecastday'][2]['pop']
forecast_2_inchesrain = parsed_json['forecast']['simpleforecast']['forecastday'][2]['qpf_allday']['in']
forecast_2_windspeed = parsed_json['forecast']['simpleforecast']['forecastday'][2]['avewind']['mph']
forecast_2_winddegrees = parsed_json['forecast']['simpleforecast']['forecastday'][2]['avewind']['degrees']
forecast_2_avghumidity= parsed_json['forecast']['simpleforecast']['forecastday'][2]['avehumidity']

#3 is the day after the day after tomorrow
forecast_3_highf = parsed_json['forecast']['simpleforecast']['forecastday'][3]['high']['fahrenheit']
forecast_3_highc = parsed_json['forecast']['simpleforecast']['forecastday'][3]['high']['celsius']
forecast_3_lowf = parsed_json['forecast']['simpleforecast']['forecastday'][3]['low']['fahrenheit']
forecast_3_lowc = parsed_json['forecast']['simpleforecast']['forecastday'][3]['low']['celsius']
forecast_3_conditions = parsed_json['forecast']['simpleforecast']['forecastday'][3]['conditions']
forecast_3_pop = parsed_json['forecast']['simpleforecast']['forecastday'][3]['pop']
forecast_3_inchesrain = parsed_json['forecast']['simpleforecast']['forecastday'][3]['qpf_allday']['in']
forecast_3_windspeed = parsed_json['forecast']['simpleforecast']['forecastday'][3]['avewind']['mph']
forecast_3_winddegrees = parsed_json['forecast']['simpleforecast']['forecastday'][3]['avewind']['degrees']
forecast_3_avghumidity= parsed_json['forecast']['simpleforecast']['forecastday'][3]['avehumidity']

#SET UP THE VARIABLE TO PRINT TO LABVIEW:

#11 delimeters for string
print ("%s@%s@%s@%s@%s@%s@%s@%s@%s@%s@%s" % (local_time, obs_city, obs_ele, obs_time, station_id,weather_conditions, relative_humidity, forecast_0_conditions, forecast_1_conditions, forecast_2_conditions, forecast_3_conditions))
#6 delimeters for numbers 
print("%s@%s@%s@%s@%s@%s" % (sunrise_h, sunrise_m, sunset_h, sunset_m, obs_lat, obs_lon))
#10 delimeters for numbers 
print ("%s@%s@%s@%s@%s@%s@%s@%s@%s@%s" % (temp_f, temp_c, wind_degrees, wind_mph, pressure_mb, pressure_in, feelslike_f, feelslike_c, precip_today_in, solarradiation))
#9 delimeters for numbers 
print ("%s@%s@%s@%s@%s@%s@%s@%s@%s" % (forecast_0_highf, forecast_0_highc, forecast_0_lowf, forecast_0_lowc, forecast_0_pop, forecast_0_inchesrain, forecast_0_windspeed, forecast_0_winddegrees, forecast_0_avghumidity))
#9 delimeters for numbers 
print ("%s@%s@%s@%s@%s@%s@%s@%s@%s" % (forecast_1_highf, forecast_1_highc, forecast_1_lowf, forecast_1_lowc, forecast_1_pop, forecast_1_inchesrain, forecast_1_windspeed, forecast_1_winddegrees, forecast_1_avghumidity))
#9 delimeters for numbers 
print ("%s@%s@%s@%s@%s@%s@%s@%s@%s" % (forecast_2_highf, forecast_2_highc, forecast_2_lowf, forecast_2_lowc, forecast_2_pop, forecast_2_inchesrain, forecast_2_windspeed, forecast_2_winddegrees, forecast_2_avghumidity))
#9 delimeters for numbers 
print ("%s@%s@%s@%s@%s@%s@%s@%s@%s" % (forecast_3_highf, forecast_3_highc, forecast_3_lowf, forecast_3_lowc, forecast_3_pop, forecast_3_inchesrain, forecast_3_windspeed, forecast_3_winddegrees, forecast_3_avghumidity))		 
print (alerts)


