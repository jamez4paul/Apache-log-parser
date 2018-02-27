# Apache log parser

## Logs format

Important parts:

* timestamp `[22/Feb/2017:18:45:02 +0000]`
* endpoint `/ubuntu.html`
* status code (right after the endpoint section) `503`
    

Format is described in detail here [here](https://httpd.apache.org/docs/1.3/logs.html).

    10.10.10.10 - - [22/Feb/2017:18:45:02 +0000] "GET /fedora.html?user=gheorghe HTTP/1.1" 401 533 "-" "python-requests/2.12.4"
    10.10.10.10 - - [22/Feb/2017:18:45:13 +0000] "GET /ubuntu.html HTTP/1.1" 200 1303 "-" "python-requests/2.12.4"
    10.10.10.10 - - [22/Feb/2017:18:45:24 +0000] "GET /centos.html#enunt HTTP/1.1" 200 1160 "-" "python-requests/2.12.4"
    10.10.10.10 - - [22/Feb/2017:18:45:34 +0000] "GET /ubuntu.html HTTP/1.1" 200 1303 "-" "python-requests/2.12.4"
    192.168.100.25 - - [22/Feb/2017:18:45:45 +0000] "GET /fedora.html?user=dorel HTTP/1.1" 401 1160 "-" "python-requests/2.12.4"
    192.168.100.25 - - [22/Feb/2017:18:45:49 +0000] "GET /fedora.html?user=gigel HTTP/1.1" 200 1160 "-" "python-requests/2.12.4"
    192.168.100.25 - - [22/Feb/2017:18:45:55 +0000] "GET /centos.html#pret HTTP/1.1" 503 1160 "-" "python-requests/2.12.4"
    192.168.100.25 - - [22/Feb/2017:18:46:06 +0000] "GET /centos.html HTTP/1.1" 200 1160 "-" "python-requests/2.12.4"
    192.168.100.25 - - [22/Feb/2017:18:46:16 +0000] "GET /fedora.html?user=dorel HTTP/1.1" 401 533 "-" "python-requests/2.12.4"
    1.2.3.4 - - [22/Feb/2017:18:46:27 +0000] "GET /fedora.html HTTP/1.1" 404 533 "-" "python-requests/2.12.4"
    1.2.3.4 - - [22/Feb/2017:18:46:37 +0000] "GET /fedora.html HTTP/1.1" 404 533 "-" "python-requests/2.12.4"
    1.2.3.4 - - [22/Feb/2017:18:46:48 +0000] "GET /centos.html HTTP/1.1" 200 1160 "-" "python-requests/2.12.4"

**Careful!** For `192.168.100.25 - - [22/Feb/2017:18:46:16 +0000] "GET /fedora.html?user=dorel HTTP/1.1" 401 533 "-" "python-requests/2.12.4"`, endpoint is only  `/fedora.html`. Query parameters and anchors are ignored.
Logs are ordered chronologically.

## Statistics format

### Intervals are determined for each endpoint.

Output components are:

* date, in the format `%year-%month-%dayT%hour:%minute`. T is a separator.
* interval duration, number of minutes - positive integer
* the endpoint
* succes rate, percentage of status codes that start with `2` (`2XX`, example `200`), from the current interval, rounded to 2 decimal places

Results are first sorted by timestamps and then lexicographically by endpoint.

Example of output for the logs above with the interval duration set at 1 minute:

    2017-02-22T18:45 1 /centos.html 50.0
    2017-02-22T18:45 1 /fedora.html 33.33
    2017-02-22T18:45 1 /ubuntu.html 100.0
    2017-02-22T18:46 1 /centos.html 100.0
    2017-02-22T18:46 1 /fedora.html 0.0


at 2 minutes:

    2017-02-22T18:45 2 /centos.html 75.0
    2017-02-22T18:45 2 /fedora.html 16.67
    2017-02-22T18:45 2 /ubuntu.html 100.0

**Careful!** A request from `18:45:59` is not cosidered in the same minute as one from `18:46:00`, even though the difference between them is less than 60 seconds

Number of minutes from an interval is sent through command line parameters, example `./log_stats --interval 2` with the implicit value of `1`.
The beginning and the end of an interval are also sent through command line parameters: `./log_stats --start 2017-02-22T18:45 --end 2017-02-22T18:48`.
