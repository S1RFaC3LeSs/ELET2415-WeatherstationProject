"""
Flask Documentation:     https://flask.palletsprojects.com/
Jinja2 Documentation:    https://jinja.palletsprojects.com/
Werkzeug Documentation:  https://werkzeug.palletsprojects.com/
This file creates your application.
"""

# from crypt import methods
import site 
import json

from app import app, Config,  mongo, Mqtt
from flask import escape, render_template, request, jsonify, send_file, redirect, make_response, send_from_directory 
from json import dumps, loads 
from werkzeug.utils import secure_filename
from datetime import datetime,timedelta, timezone
from os import getcwd
from os.path import join, exists
from time import time, ctime
from math import floor
 



#####################################
#   Routing for your application    #
#####################################

@app.route('/api/weatherstation/get/<start>/<end>', methods=['GET']) 
def get_all(start,end):   
    '''RETURNS ALL THE DATA FROM THE DATABASE THAT EXIST IN BETWEEN THE START AND END TIMESTAMPS'''
    start = int(start)
    end = int(end)
    print(f"Start Date: {start}")
    print(f"End Date: {end}")
    print(type(start))
    print(type(end))
    if request.method == "GET":
        '''Add your code here to complete this route'''
        try:
            item = mongo.getAllInRange(start,end)
            data = list(item)
            if data:
                return jsonify({"status":"found","data": data})
        except Exception as e:
            print(f"get_all error: f{str(e)}") 
    # FILE DATA NOT EXIST
    return jsonify({"status":"not found","data":[]})


@app.route('/api/mmar/temperature/<start>/<end>', methods=['GET']) 
def get_temperature_mmar(start, end):
        '''RETURNS MIN, MAX, AVG AND RANGE FOR TEMPERATURE. THAT FALLS WITHIN THE START AND END DATE RANGE'''
        start= int(start)
        end= int(end)
        print(f"Start Date: {start}")
        print(f"End Date: {end}")
        print(type(start))
        print(type(end))
        if request.method == "GET":
            try:
                item= mongo.temperatureMMAR(start, end)
                print(f"Item: {item}")
                if item:

                    return jsonify({"status":"found","data":item})
                    print("No data")
            except Exception as e:
                msg = str(e)
                print(f"get_all error: f{str(e)}")

        return jsonify({"status":"not found","data":[]})


@app.route('/api/mmar/humidity/<start>/<end>', methods=['GET'])
def get_humidity_mmar(start, end):
        '''RETURNS MIN, MAX, AVG AND RANGE FOR HUMIDITY. THAT FALLS WITHIN THE START AND END DATE RANGE'''
        start= int(start)
        end= int(end)

        if request.method == "GET":
            try:
                item= mongo.humidityMMAR(start, end)
                if item:
                    return jsonify({"status":"found","data":item})
            
            except Exception as e:
                msg = str(e)
                print(f"get_all error: f{str(e)}")

            return jsonify({"status":"not found","data":[]})


@app.route('/api/mmar/pressure/<start>/<end>', methods=['GET'])
def get_pressure_mmar(start, end):
        '''RETURNS MIN, MAX, AVG AND RANGE FOR PRESSURE. THAT FALLS WITHIN THE START AND END DATE RANGE'''
        start= int(start)
        end= int(end)
        print(f"Start Date: {start}")
        print(f"End Date: {end}")

        if request.method == "GET":
            try:
                item= mongo.pressureMMAR(start, end)
                if item:
                    return jsonify({"status":"found","data":item})
            
            except Exception as e:
                msg = str(e)
                print(f"get_all error: f{str(e)}")

            return jsonify({"status":"not found","data":[]})


@app.route('/api/mmar/moisture/<start>/<end>', methods=['GET'])
def get_soil_mmar(start, end):
        '''RETURNS MIN, MAX, AVG AND RANGE FOR SOIL MOISTURE. THAT FALLS WITHIN THE START AND END DATE RANGE'''
        start= int(start)
        end= int(end)

        if request.method == "GET":
            try:
                item= mongo.soilMMAR(start, end)
                if item:
                    return jsonify({"status":"found","data":item})
            
            except Exception as e:
                msg = str(e)
                print(f"get_all error: f{str(e)}")

            return jsonify({"status":"not found","data":[]})


@app.route('/api/frequency/<variable>/<start>/<end>', methods=['GET'])
def get_freq_distro(variable,start, end):
        '''RETURNS THE FREQUENCY DISTROBUTION FOR A SPECIFIED VARIABLE WITHIN THE START AND END DATE RANGE'''
        start= int(start)
        end= int(end)
        variable= str(variable)

        if request.method == "GET":
            try:
                item= mongo.frequencyDistro(variable, start, end)
                if item:
                    return jsonify({"status":"found","data":item})
            
            except Exception as e:
                msg = str(e)
                print(f"get_all error: f{str(e)}")

            return jsonify({"status":"not found","data":[]})
   


@app.route('/api/file/get/<filename>', methods=['GET']) 
def get_images(filename):   
    '''RETURNS REQUESTED FILE FROM UPLOADS FOLDER'''
   
    if request.method == "GET":
        '''Add your code here to complete this route'''
        directory   = join( getcwd(), Config.UPLOADS_FOLDER) 
        filePath    = join( getcwd(), Config.UPLOADS_FOLDER, filename) 

        # RETURN FILE IF IT EXISTS IN FOLDER
        if exists(filePath):        
            return send_from_directory(directory, filename)
        
        # FILE DOES NOT EXIST
        return jsonify({"status":"file not found"}), 404



@app.route('/api/file/upload',methods=["POST"])  
def upload():
    '''SAVES A FILE TO THE UPLOADS FOLDER'''
    
    if request.method == "POST": 
        file     = request.files['file']
        filename = secure_filename(file.filename)
        file.save(join(getcwd(),Config.UPLOADS_FOLDER , filename))
        return jsonify({"status":"File upload successful", "filename":f"{filename}" })

 


###############################################################
# The functions below should be applicable to all Flask apps. #
###############################################################

# @app.route('/<file_name>.txt')
# def send_text_file(file_name):
#     """Send your static text file."""
#     file_dot_text = file_name + '.txt'
#     return app.send_static_file(file_dot_text)


@app.after_request
def add_header(response):
    """
    Add headers to both force latest IE rendering engine or Chrome Frame,
    and also tell the browser not to cache the rendered page. If we wanted
    to we could change max-age to 600 seconds which would be 10 minutes.
    """
    response.headers['X-UA-Compatible'] = 'IE=Edge,chrome=1'
    response.headers['Cache-Control'] = 'public, max-age=0'
    return response

@app.errorhandler(405)
def page_not_found(error):
    """Custom 404 page."""    
    return jsonify({"status": 404}), 404



