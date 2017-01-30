import re
from subprocess import Popen
from subprocess import PIPE
import sys

# The expected output from curl as a regular expression
curl_output_expected = ("GET / HTTP/1\\.0\r\n"
                        "Host: localhost:\\d+\r\n"
                        "User-Agent: curl/\\d+\\.\\d+\\.\\d+\r\n"
                        "Accept: \\*/\\*\r\n")

# We start off with the assumption that the test will succeed
ec = 0

# Open the web server in a subprocess
webserver = Popen(["./webserver", "port_config"], stdout=PIPE)

# Query the web server using curl
curl = Popen(["curl", "-0", "-s", "localhost:1234"], stdout=PIPE)
curl_output = curl.communicate()[0]
curl_output_pattern = re.compile(curl_output_expected)
if not curl_output_pattern.match(curl_output):
    ec = 1

# Close the webserver
webserver.terminate()

# Return 0 if the test succeeded or some other value on failure
if ec == 0:
    sys.stdout.write("INTEGRATION TEST SUCCEEDED\n")
    sys.exit(0)
else:
    sys.stdout.write("INTEGRATION TEST FAILED\n")
    sys.stderr.write("\nExpected output from curl (regular expression)\n")
    sys.stderr.write("==========\n")
    sys.stderr.write(curl_output_expected)
    sys.stderr.write("==========\n")
    sys.stderr.write("\nActual output from curl\n")
    sys.stderr.write("==========\n")
    sys.stderr.write(curl_output)
    sys.stderr.write("==========\n\n")
    sys.exit(ec)

