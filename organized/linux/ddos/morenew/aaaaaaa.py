import socket
import os
import string
import select
import sys
import time
import urllib
import random
import hashlib
import urllib2
import threading
from datetime import datetime


class Spammer(threading.Thread):

    def __init__(self):

        threading.Thread.__init__(self)

        self.Signup       = "http://linkhere.com/cf-usersignup.asp"
        self.Login        = "http://linkhere.com/cf-userlogin.php"
        self.Key          = "N7B5W8W1Y5B1R5VWVZ"
        self.RandomData   = ""
        self.Username     = ""
        self.Enemy        = ""
        self.Message      = "Test"
        self.Room         = "faroff"
        self.Hash         = ""
        self.RandomLength = 10
        self.socket       = None
        self.Datas        = ""
        self.Requests     = ""
        self.Connected = False
        self.ChatBomb     = False
        self.HTMLRead     = ""
        self.DataRCV      = ""
        self.SignupData   = {'strGender' : 'M',
                             'strUsername' : '',
                             'strEmail' : '',
                             'intColorEye' : '91294',
                             'intColorSkin' : '15388042',
                             'intColorHair' : '6180663',
                             'strDOB' : '3/12/2002',
                             'strPassword' : '',
                             'HairID' : '52',
                             'intAge' : '13',
                             'ClassID' : '2'}
        self.LoginData   = {'unm' : '',
                            'pwd' : ''}

        self.SpammerSignup()

    def println(self, Type, text):
        print "["+str(self.getTime())+"]"+"["+str(Type)+"] "+str(text)

    def getTime(self):
        TIME = str(datetime.now())[11:].split(":")
        TIME = TIME[0]+":"+TIME[1]+":"+TIME[2][:2]
        return str(TIME)

    def sendData(self, data):

        if self.Connected:
            self.socket.send(data + chr(0))
            print ("Send: {}".format(data))

    def sendMessage(self):
        while True:
            time.sleep(1.2)
            if self.ChatBomb == False:
                self.sendData("%xt%zm%emotea%1%dance%")
                self.sendData("%xt%zm%message%1%"+ self.Message +"%zone%")
                self.sendData("%xt%zm%whisper%1%"+ self.Message +"%"+ self.Enemy +"%")
            else:
                self.sendData("%xt%zm%emotea%1%dance%")
                self.sendData("%xt%zm%message%1%ndjsnfsdniofnenfieifnesionfesinfiemfkdslfnfenofiesofneinifesnionfesiniofensinfioesnfnessionfiesnifnesionfe000000000000000000000000000000000000000000000000000000000000000000000000000000000000000fdsoinfeiofioeniofneinfiesnifnesnfesfdsnfieiofejiofesionefsninfesinfesinifesnnfoeisnfiesniofneiosnfioesnioefsnionfesionfesionfioesniofesnioefsniofesnionfesionfioesnifoesniofesnioefsniofnesionfeisnofesniofesnfnosnfieofien0000000000000000000000000000000ndjsnfsdniofnenfieifnesionfesinfiemfkdslfnfenofiesofneinifesnionfesiniofensinfioesnfnessionfiesnifnesionfe000000000000000000000000000000000000000000000000000000000000000000000000000000000000000fdsoinfeiofioeniofneinfiesnifnesnfesfdsnfieiofejiofesionefsninfesinfesinifesnnfoeisnfiesniofneiosnfioesnioefsnionfesionfesionfioesniofesnioefsniofesnionfesionfioesnifoesniofesnioefsniofnesionfeisnofesniofesnndjsnfsdniofnenfieifnesionfesinfiemfkdslfnfenofiesofneinifesnionfesiniofensinfioesnfnessionfiesnifnesionfe000000000000000000000000000000000000000000000000000000000000000000000000000000000000000fdsoinfeiofioeniofneinfiesnifnesnfesfdsnfieiofejiofesionefsninfesinfesinifesnnfoeisnfiesniofneiosnfioesnioefsnionfesionfesionfioesniofesnioefsniofesnionfesionfioesnifoesniofesnioefsniofnesionfeisnofesniofesnndjsnfsdniofnenfieifnesionfesinfiemfkdslfnfenofiesofneinifesnionfesiniofensinfioesnfnessionfiesnifnesionfe000000000000000000000000000000000000000000000000000000000000000000000000000000000000000fdsoinfeiofioeniofneinfiesnifnesnfesfdsnfieiofejiofesionefsninfesinfesinifesnnfoeisnfiesniofneiosnfioesnioefsnionfesionfesionfioesniofesnioefsniofesnionfesionfioesnifoesniofesnioefsniofnesionfeisnofesniofesnndjsnfsdniofnenfieifnesionfesinfiemfkdslfnfenofiesofneinifesnionfesiniofensinfioesnfnessionfiesnifnesionfe000000000000000000000000000000000000000000000000000000000000000000000000000000000000000fdsoinfeiofioeniofneinfiesnifnesnfesfdsnfieiofejiofesionefsninfesinfesinifesnnfoeisnfiesniofneiosnfioesnioefsnionfesionfesionfioesniofesnioefsniofesnionfesionfioesnifoesniofesnioefsniofnesionfeisnofesniofesn%zone%")
                self.sendData("%xt%zm%whisper%1%ndjsnfsdniofnenfieifnesionfesinfiemfkdslfnfenofiesofneinifesnionfesiniofensinfioesnfnessionfiesnifnesionfe000000000000000000000000000000000000000000000000000000000000000000000000000000000000000fdsoinfeiofioeniofneinfiesnifnesnfesfdsnfieiofejiofesionefsninfesinfesinifesnnfoeisnfiesniofneiosnfioesnioefsnionfesionfesionfioesniofesnioefsniofesnionfesionfioesnifoesniofesnioefsniofnesionfeisnofesniofesnfnosnfieofien0000000000000000000000000000000ndjsnfsdniofnenfieifnesionfesinfiemfkdslfnfenofiesofneinifesnionfesiniofensinfioesnfnessionfiesnifnesionfe000000000000000000000000000000000000000000000000000000000000000000000000000000000000000fdsoinfeiofioeniofneinfiesnifnesnfesfdsnfieiofejiofesionefsninfesinfesinifesnnfoeisnfiesniofneiosnfioesnioefsnionfesionfesionfioesniofesnioefsniofesnionfesionfioesnifoesniofesnioefsniofnesionfeisnofesniofesnndjsnfsdniofnenfieifnesionfesinfiemfkdslfnfenofiesofneinifesnionfesiniofensinfioesnfnessionfiesnifnesionfe000000000000000000000000000000000000000000000000000000000000000000000000000000000000000fdsoinfeiofioeniofneinfiesnifnesnfesfdsnfieiofejiofesionefsninfesinfesinifesnnfoeisnfiesniofneiosnfioesnioefsnionfesionfesionfioesniofesnioefsniofesnionfesionfioesnifoesniofesnioefsniofnesionfeisnofesniofesnndjsnfsdniofnenfieifnesionfesinfiemfkdslfnfenofiesofneinifesnionfesiniofensinfioesnfnessionfiesnifnesionfe000000000000000000000000000000000000000000000000000000000000000000000000000000000000000fdsoinfeiofioeniofneinfiesnifnesnfesfdsnfieiofejiofesionefsninfesinfesinifesnnfoeisnfiesniofneiosnfioesnioefsnionfesionfesionfioesniofesnioefsniofesnionfesionfioesnifoesniofesnioefsniofnesionfeisnofesniofesnndjsnfsdniofnenfieifnesionfesinfiemfkdslfnfenofiesofneinifesnionfesiniofensinfioesnfnessionfiesnifnesionfe000000000000000000000000000000000000000000000000000000000000000000000000000000000000000fdsoinfeiofioeniofneinfiesnifnesnfesfdsnfieiofejiofesionefsninfesinfesinifesnnfoeisnfiesniofneiosnfioesnioefsnionfesionfesionfioesniofesnioefsniofesnionfesionfioesnifoesniofesnioefsniofnesionfeisnofesniofesn%"+ self.Enemy +"%")
            
            
    def run(self):

        while self.Connected:

            data = self.socket.recv(1024)

            if data.startswith("<cross-domain-policy><allow-access-from domain='*'"):
                self.sendData("<msg t='sys'><body action='login' r='0'><login z='zone_master'><nick><![CDATA["+self.Key+"~"+self.Username+"]]></nick><pword><![CDATA["+self.Hash+"]]></pword></login></body></msg>")

            elif "loginResponse" in data:
                self.sendData("%xt%zm%firstJoin%1%")
                self.sendData("%xt%zm%moveToCell%0%Enter%Spawn%")
                self.sendData("%xt%zm%retrieveInventory%0%")
                self.sendData("%xt%zm%cmd%0%tfer%"+self.Username+"%"+self.Room+"%")

                # Start messages
                threading.Thread(target=self.sendMessage).start()

            elif "You are muted!" in data:
                self.socket.close()
                self.Connected = False
                self.SpammerSignup()
            elif "You are muted! Chat privileges have been temporarily revoked." in data:
                self.socket.close()
                self.Connected = False
                self.SpammerSignup()
            elif "You need at least 100ACs to send a message in world channel." in data:
                self.socket.close()
                self.Connected = False
                self.SpammerSignup()
            elif "Action taken too quickly, try again in a moment." in data:
                self.socket.close()
                self.Connected = False
                self.SpammerSignup()
            print ("Received: {}".format(data))


    def ConnectBOT(self):
        try:

            self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.socket.connect(("145.32.140.150", 5588))
            print self.SignupData['strUsername']
            print self.SignupData['strPassword']

            self.Connected = True

        except Exception as Error:
            print Error

    def SpammerLogin(self):

        # headers
        headers = {
            "Host": "linkhere.com",
            "User-Agent": "Mozilla/5.0 (Windows NT 10.0; WOW64; rv:43.0) Gecko/20100101 Firefox/43.0",
            "Accept": "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8",
            "Accept-Language": "en-GB,en;q=0.5",
            "Accept-Encoding": "deflate",
            "Connection": "keep-alive",
            "If-Modified-Since": "*",
            "Referer": "http://linkhere.com/gamefiles/Loader.swf",
            "Content-type": "application/x-www-form-urlencoded"
        }

        # Params
        params = {
            "strUsername": self.Username,
            "user": self.Username,
            "strPassword": self.Hash,
            "pass": self.Hash
        }

        if "bSuccess" in response:
            self.ConnectBOT()

    def SpammerSignup(self):

        # headers
        headers = {
            "Host": "linkhere.com",
            "User-Agent": "Mozilla/5.0 (Windows NT 10.0; WOW64; rv:43.0) Gecko/20100101 Firefox/43.0",
            "Accept": "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8",
            "Accept-Language": "en-GB,en;q=0.5",
            "Accept-Encoding": "deflate",
            "Cookie": "__cfduid=d5fa16ed531dbf04e7786cdc14104c0551447736009; login=Enter; google=DcS54fsd321ad8asd131",
            "Connection": "keep-alive",
            "If-Modified-Since": "*",
            "Referer": "http://linkhere.com/gamefiles/newuser/Register6.swf",
            "Content-type": "application/x-www-form-urlencoded"
        }

        # Params
        params = {
            "strUsername": self.randomString(9),
            "strPassword": self.randomString(9),
            "strEmail": self.randomString(12) + "@live.com",
            "strGender": "M",
            "ClassID": "2",
            "HairID": "52",
            "intAge": "10",
            "strDOB": "6/22/2002", # Temp
            "intColorEye": "91294",
            "intColorSkin": "15388042",
            "intColorHair": "6180663"
        }

        # Encode
        encode = urllib.urlencode(params).decode("cp1252")

        # Post
        request = urllib2.Request(self.Signup, encode, headers)

        # Urlopen
        response = urllib2.urlopen(request).read().decode("cp1252")

        if "status=Success" in response:

            self.Username = params["strUsername"]
            self.Hash     = params["strPassword"]

            # Login
            self.SpammerLogin()


    def randomString(self, length):
        return ''.join(random.choice(string.ascii_lowercase) for _ in range(length))

if __name__ == '__main__':

    # Spawn bots
    for _ in range(2):
        Spammer().start()
