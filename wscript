# vim:ft=python

import Options
from Configure import conf
from config_c import parse_flags
import UnitTest, Utils, Build, misc
import os.path

VERSION = '0.1.0'
APPNAME = 'ecru'

srcdir = '.'
blddir = 'output'

Utils.waf_version(mini='1.5.0', maxi='1.5.9')

def set_options(opt):
    opt.add_option('--debug', action='store_true', default=False,
            help='Compile with debug symbols')

    opt.add_option('--mandir', type='string', 
            help='path to install man pages')

def configure(conf):
    conf.check_tool('misc')    

    conf.env["APPNAME"] = APPNAME
    conf.env["VERSION"] = VERSION
    conf.env["MANDIR"] = os.path.join(conf.env["PREFIX"], "share", "man")

    conf.define('APP_NAME', APPNAME)
    conf.define('APP_VERSION', VERSION)
    conf.write_config_header('src/ecru_version.h')

    conf.check_tool('g++')
    if not conf.env['CXX']:
        conf.fatal('g++ not found')

    conf.find_program('xmlrpc-c-config', var='XMLRPC-C-CONFIG')
    if not conf.env['XMLRPC-C-CONFIG']:
        conf.fatal("xmlrpc-c-config wasn't found")

    conf.env.append_value('CXXFLAGS', '-Wall -Werror')

    if Options.options.debug:
        conf.env.append_value('CXXFLAGS', '-g')

    conf.check_cfg(atleast_pkgconfig_version='0.0.0')
    conf.check_cfg(package='libconfig++', uselib_store='LIBCONFIGXX', args='--cflags --libs')
    conf.check_cfg(package='glibmm-2.4', uselib_store='GLIBMM', args='--cflags --libs')
    ret = Utils.cmd_output("%s c++2 client --cflags --libs --ldflags" % (conf.env['XMLRPC-C-CONFIG']))
    parse_flags(ret, 'XMLRPC', conf.env)

    if Options.options.mandir:
        conf.env["MANDIR"] = Options.options.mandir
    else:
        conf.env["MANDIR"] = os.path.join(conf.env["PREFIX"], "man")

    # for unit tests
    try:
        conf.check_cfg(package='cppunit', uselib_store='CPPUNIT', args='--cflags --libs')
    except ValueError:
        pass

    if 'LIB_CPPUNIT' in conf.env:
        Utils.pprint('CYAN', "To run unit tests use 'waf check'")
    else:
        print "cppunit not found, no unit tests"

def build(bld):
    subdirs = 'src'
    if bld.env['LIB_CPPUNIT']:
        subdirs += ' tests'
    bld.add_subdirs(subdirs)
    
    
    bld.install_files(os.path.join(bld.env["MANDIR"], "man1"),
            "doc/man/man1/*.gz")
           

def shutdown():
    # Unit tests are run when "check" target is used
    ut = UnitTest.unit_test()
    ut.change_to_testfile_dir = True
    ut.run()
    ut.print_results()

import Scripting
def dont_dist(name, src, build_dir):
    excludes = '.git .gitignore'
    exclude_exts = '~ .rej .orig .pyc .pyo .bak .tar.bz2 .core'.split()

    if (name.startswith(".waf-")
            or name in excludes
            or name == build_dir
            or (src == '.' and name == Options.lockfile)
            ):
        return True

    for ext in exclude_exts:
        if name.endswith(ext):
            return True

    return False

Scripting.dont_dist = dont_dist
