#include <Arduino.h>

#include "SettingsManager.h"
#include "ServerManager.h"
#include "NightscoutManager.h"
#include "DisplayManager.h"
#include "BGDisplayManager.h"
#include "PeripheryManager.h"
#include "globals.h"

#include "improv_consume.h"

float apModeHintPosition = 32;

void setup()
{

    pinMode(15, OUTPUT);
    digitalWrite(15, LOW);
    delay(2000);
    Serial.begin(115200);
    // Serial.setDebugOutput(true);

    DisplayManager.setup();
    SettingsManager.setup();
    if (!SettingsManager.loadSettingsFromFile())
    {
        DisplayManager.showFatalError("Error loading software, please reinstall");
    }

    DisplayManager.applySettings();

    DisplayManager.HSVtext(2, 6, "Loading", true, 0);

    ServerManager.setup();
    NightscoutManager.setup();
    BGDisplayManager.setup();
    PeripheryManager.setup();

    DEBUG_PRINTLN("Setup done");
    String welcomeMessage = "Nightscout clock   " + ServerManager.myIP.toString();
    DisplayManager.scrollColorfulText(welcomeMessage);

    DisplayManager.clearMatrix();
    DisplayManager.setTextColor(COLOR_WHITE);
    DisplayManager.printText(0, 6, "Connect", CENTER, 2);
}

void showJoinAP()
{
    String hint = "Join " + SettingsManager.settings.hostname + " Wi-fi network and go to http://" + ServerManager.myIP.toString() + "/";

    if (apModeHintPosition < -240)
    {
        apModeHintPosition = 32;
        DisplayManager.clearMatrix();
    }

    DisplayManager.HSVtext(apModeHintPosition, 6, hint.c_str(), true, 1);
    apModeHintPosition -= 0.18;
}

void loop()
{
    if (ServerManager.isConnected)
    {
        ServerManager.tick();
        NightscoutManager.tick();
        BGDisplayManager.tick();
        if (NightscoutManager.hasNewData(BGDisplayManager.getLastDisplayedGlucoseEpoch()))
        {
            DEBUG_PRINTLN("We have new data");
            BGDisplayManager.showData(NightscoutManager.getInstance().getGlucoseData());
        }
    }
    else if (ServerManager.isInAPMode)
    {
        showJoinAP();
    }

    checckForImprovWifiConnection();

    DisplayManager.tick();
    PeripheryManager.tick();
}
