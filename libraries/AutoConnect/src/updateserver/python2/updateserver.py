#!python2.*

"""update server.
"""

from __future__ import absolute_import
import argparse
import hashlib
import httplib
import CGIHTTPServer, SimpleHTTPServer, BaseHTTPServer
import json
import logging
import os
import re
import socket
import time
import urllib2, urllib, urlparse
from itertools import imap
from io import open


class UpdateHttpServer(object):
    def __init__(self, port, bind, catalog_dir):
        def handler(*args):
            UpdateHTTPRequestHandler(catalog_dir, *args)
        httpd = BaseHTTPServer.HTTPServer((bind, port), handler)
        sa = httpd.socket.getsockname()
        logger.info('http server starting {0}:{1} {2}'.format(sa[0], sa[1], catalog_dir))
        try:
            httpd.serve_forever()
        except KeyboardInterrupt:
            logger.info('Shutting down...')
            httpd.socket.close()


class UpdateHTTPRequestHandler(BaseHTTPServer.BaseHTTPRequestHandler):
    def __init__(self, catalog_dir, *args):
        self.catalog_dir = catalog_dir
        BaseHTTPServer.BaseHTTPRequestHandler.__init__(self, *args)

    def do_GET(self):
        request_path = urlparse.urlparse(self.path)
        if request_path.path == '/_catalog':
            err = ''
            query = urlparse.urlparse(self.path).query
            try:
                op = urlparse.parse_qs(query)['op'][0]
                if op == 'list':
                    try:
                        path = urlparse.parse_qs(query)['path'][0]
                    except KeyError:
                        path = '.'
                    full_path = os.path.join(self.catalog_dir, path)
                    if os.path.commonprefix((os.path.realpath(full_path), self.catalog_dir)) == self.catalog_dir:
                        self.__send_dir(full_path)
                        result = True
                    else:
                        err = 'Path traversal detected'
                        result = False
                else:
                    err = '{0} unknown operation'.format(op)
                    result = False
            except KeyError:
                err = '{0} invaid catalog request'.format(self.path)
                result = False
            if not result:
                logger.info(err)
                self.send_response(httplib.FORBIDDEN, err)
                self.end_headers()
        else:
            self.__send_file(self.path)

    def __check_header(self):
        ex_headers_templ = ['x-*-STA-MAC', 'x-*-AP-MAC', 'x-*-FREE-SPACE', 'x-*-SKETCH-SIZE', 'x-*-SKETCH-MD5', 'x-*-CHIP-SIZE', 'x-*-SDK-VERSION']
        ex_headers = []
        ua = re.match('(ESP8266|ESP32)-http-Update', self.headers.get('User-Agent'))
        if ua:
            arch = ua.group().split('-')[0]
            ex_headers = list(imap(lambda x: x.replace('*', arch), ex_headers_templ))
        else:
            logger.info('User-Agent {0} is not HTTPUpdate'.format(ua))
            return False
        for ex_header in ex_headers:
            if ex_header not in self.headers:
                logger.info('Missing header {0} to identify a legitimate request'.format(ex_header))
                return False
        return True

    def __send_file(self, path):
        if not self.__check_header():
            self.send_response(httplib.FORBIDDEN, 'The request available only from ESP8266 or ESP32 http updater.')
            self.end_headers()
            return

        filename = os.path.join(self.catalog_dir, path.lstrip('/'))
        logger.debug('Request file:{0}'.format(filename))
        try:
            fsize = os.path.getsize(filename)
            self.send_response(httplib.OK)
            self.send_header('Content-Type', 'application/octet-stream')
            self.send_header('Content-Disposition', 'attachment; filename=' + os.path.basename(filename))
            self.send_header('Content-Length', fsize)
            self.send_header('x-MD5', get_MD5(filename))
            self.end_headers()
            f = open(filename, 'rb')
            self.wfile.write(f.read())
            f.close()
        except Exception, e:
            err = unicode(e)
            logger.error(err)
            self.send_response(httplib.INTERNAL_SERVER_ERROR, err)
            self.end_headers()

    def __send_dir(self, path):
        content = dir_json(path)
        d = json.dumps(content).encode('UTF-8', 'replace')
        logger.debug(d)
        self.send_response(httplib.OK)
        self.send_header('Content-Type', 'application/json')
        self.send_header('Content-Length', unicode(len(d)))
        self.end_headers()
        self.wfile.write(d)


def dir_json(path):
    d = list()
    for entry in os.listdir(path):
        e = {'name': entry}
        if os.path.isdir(entry):
            e['type'] = "directory"
        else:
            e['type'] = "file"
            if os.path.splitext(entry)[1] == '.bin':
                fn = os.path.join(path, entry)
                try:
                    f = open(fn, 'rb')
                    c = f.read(1)
                    f.close()
                except Exception, e:
                    logger.info(unicode(e))
                    c = '\x00'
                if c == '\xe9':
                    e['type'] = "bin"
                    mtime = os.path.getmtime(fn)
                    e['date'] = time.strftime('%x', time.localtime(mtime))
                    e['time'] = time.strftime('%X', time.localtime(mtime))
                    e['size'] = os.path.getsize(fn)
        d.append(e)
    return d


def get_MD5(filename):
    try:
        f = open(filename, 'rb')
        bin_file = f.read()
        f.close()
        md5 = hashlib.md5(bin_file).hexdigest()
        return md5
    except Exception, e:
        logger.error(unicode(e))
        return None


def run(port=8000, bind='127.0.0.1', catalog_dir='', log_level=logging.INFO):
    logging.basicConfig(level=log_level)
    UpdateHttpServer(port, bind, catalog_dir)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument('--port', '-p', action='store', default=8000, type=int,
                        help='Port number [default:8000]')
    parser.add_argument('--bind', '-b', action='store', default=socket.gethostbyname(socket.gethostname()),
                        help='Specifies address to which it should bind. [default:HOST IP or 127.0.0.1]')
    parser.add_argument('--catalog', '-d', action='store', default=os.getcwdu(),
                        help='Catalog directory')
    parser.add_argument('--log', '-l', action='store', default='INFO',
                        help='Logging level')
    args = parser.parse_args()
    loglevel = getattr(logging, args.log.upper(), None)
    if not isinstance(loglevel, int):
        raise ValueError('Invalid log level: %s' % args.log)
    logger = logging.getLogger(__name__)
    run(args.port, args.bind, args.catalog, loglevel)
