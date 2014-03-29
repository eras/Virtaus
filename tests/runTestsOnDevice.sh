#!/bin/bash

# Script for running tests. That's for specifying just one argument in QtCreator's configuration
/usr/bin/tst-harbour-virtaus -input /usr/share/tst-harbour-virtaus

# When you'll get some QML components in the main app, you'll need to import them to the test run
# /usr/bin/tst-harbour-virtaus -input /usr/share/tst-harbour-virtaus -import /usr/share/harbour-virtaus/qml/components