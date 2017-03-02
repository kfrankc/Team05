import re
from subprocess import Popen
from subprocess import PIPE
import socket
import sys

# The expected output from curl's echo request as a regular expression
curl_output_expected_echo = [
    "GET /echo HTTP/1\\.0\r\n",
    "Host: localhost:\\d+\r\n",
    "User-Agent: curl/\\d+\\.\\d+\\.\\d+\r\n",
    "Accept: \\*/\\*\r\n"
]

# The expected output from curl's echo request as a regular expression
curl_output_expected_echo2 = [
    "GET /echo/ HTTP/1\\.0\r\n",
    "Host: localhost:\\d+\r\n",
    "User-Agent: curl/\\d+\\.\\d+\\.\\d+\r\n",
    "Accept: \\*/\\*\r\n"
]

# The expected output from curl's echo request as a regular expression
curl_output_expected_static_image = [
    "HTTP/1\\.0 200 OK\r\n",
    "Content-Length: \\d+\r\n",
    "Content-Type: image/jpeg\r\n"
]

# The expected output from the multithreading test
expected_multithreaded_output = (
    "HTTP/1.0 200 OK\r\n"
    "Content-Length: 22\r\n"
    "Content-Type: text/plain\r\n"
    "\r\n"
    "GET /echo HTTP/1.0\r\n"
    "\r\n"
)

# We start off with the assumption that the test will succeed
ec = 0

# Open the web server in a subprocess
webserver = Popen(["./webserver", "example_config"], stdout=PIPE)

# Request an echo from the webserver using curl
curl = Popen(["curl", "-0", "-s", "localhost:2020/echo"], stdout=PIPE)
curl_output = curl.communicate()[0].decode()
sys.stdout.write("\nServer output for integration test 1 - echo handler\n")
sys.stdout.write("==========\n")
sys.stdout.write(curl_output)
sys.stdout.write("==========\n\n")
for string in curl_output_expected_echo:
    pattern = re.compile(string)
    if not pattern.search(curl_output):
        ec = 1
        sys.stdout.write("FAILED to match the following regular expression:\n")
        sys.stdout.write("  " + string + "\n")

# Request an echo from the webserver using curl
curl = Popen(["curl", "-0", "-s", "localhost:2020/echo/"], stdout=PIPE)
curl_output = curl.communicate()[0].decode()
sys.stdout.write("\nServer output for integration test 2 - syntax check\n")
sys.stdout.write("==========\n")
sys.stdout.write(curl_output)
sys.stdout.write("==========\n\n")
for string in curl_output_expected_echo2:
    pattern = re.compile(string)
    if not pattern.search(curl_output):
        ec = 1
        sys.stdout.write("FAILED to match the following regular expression:\n")
        sys.stdout.write("  " + string + "\n")

# Request a file from the webserver using curl
curl = Popen(["curl", "-0", "-s", "localhost:2020/test_file"], stdout=PIPE)
curl_output = curl.communicate()[0].decode()
sys.stdout.write("\nServer output for integration test 3 - file handler\n")
sys.stdout.write("==========\n")
sys.stdout.write(curl_output)
sys.stdout.write("==========\n\n")
if not curl_output == "TEST\n":
    ec = 1
    sys.stdout.write("FAILED to match the following regular expression:\n")
    sys.stdout.write("  TEST\n")

# Request an image file from the webserver using curl
curl = Popen(["curl", "-0", "-s", "-I", "localhost:2020/bunny.jpg"], stdout=PIPE)
curl_output = curl.communicate()[0].decode()
sys.stdout.write("\nServer output for integration test 4 - bunny image\n")
sys.stdout.write("==========\n")
sys.stdout.write(curl_output)
sys.stdout.write("==========\n\n")
for string in curl_output_expected_static_image:
    pattern = re.compile(string)
    if not pattern.search(curl_output):
        ec = 1
        sys.stdout.write("FAILED to match the following regular expression:\n")
        sys.stdout.write("  " + string + "\n")

# Request a file from the webserver using curl that does not exist
curl = Popen(["curl", "-0", "-s", "localhost:2020/doesnotexist"], stdout=PIPE)
curl_output = curl.communicate()[0].decode()
sys.stdout.write("\nServer output for integration test 5 - handler dne\n")
sys.stdout.write("==========\n")
sys.stdout.write(curl_output)
sys.stdout.write("\n==========\n\n")
if not curl_output == "<html><head><title>Not Found</title></head><body><h1>404 Not Found</h1></body></html>":
    ec = 1
    sys.stdout.write("FAILED to match the following expression:\n")
    sys.stdout.write("  <html><head><title>Not Found</title></head><body><h1>404 Not Found</h1></body></html>\n")

# Test the multithreading capabilities of the webserver
s1 = socket.socket()
s1.connect(("localhost", 2020))
s1.send("GET /echo HT")
s2 = socket.socket()
s2.connect(("localhost", 2020))
s2.send("GET /echo HTTP/1.0\r\n\r\n")
s2.settimeout(1)
multithreaded_output = s2.recv(1024)
s1.close()
s2.close()
sys.stdout.write("\nServer output for integration test 6 - multithreading\n")
sys.stdout.write("==========\n")
sys.stdout.write(multithreaded_output)
sys.stdout.write("\n==========\n\n")
if not multithreaded_output == expected_multithreaded_output:
    ec = 1
    sys.stdout.write("FAILED to match the following expression:\n")
    sys.stdout.write(expected_multithreaded_output)

# Close the webserver
webserver.terminate()

# Return 0 if the test succeeded or some other value on failure
if ec == 0:
    sys.stdout.write("INTEGRATION TEST SUCCEEDED\n")
    sys.exit(0)
else:
    sys.stdout.write("INTEGRATION TEST FAILED\n")
    sys.exit(ec)

