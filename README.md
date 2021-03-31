# ARGOS 
# The Articulating Robotic Gaze Operated System

Interdisciplinary Senior Design Project for Stevens Institute of Technology 2021
Created by Carmine Freda, John Gazzara, Nicholas Gray, Ryan Palmer
Under advisement of Biruk Gebre

Arduino credit to Carmine Freda
Python Credit to John Gazzara

This system functions by one Arduino connected to a PC reading Gyroscope values. 
This is known as the "server"

The other Arduino is connected to a Raspberry Pi that is operating within 'ARGOS'
This is known as the "clent"

The two Arduinos communicate using the Python Socket library. A Python script must be running on both the client and server devices for commuication to occur. 
Data flows when all four programs are running simultaneously.

Arduino Server -> Python Server ---->Internet----> Python Client -> Arduino Client
aka
Gyroscope -> PC ---->Internet----> Raspberry Pi -> Articulation System
