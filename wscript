# vim:ft=python

from logging import fatal

from Configure import conf
import UnitTest, Utils, Build, misc

VERSION = '0.1.0'
APPNAME = 'ecru'

srcdir = '.'
blddir = 'output'

Utils.waf_version(mini='1.5.0', maxi='1.5.9')

def set_options(opt):
    print '  setting the options'

def configure(conf):
    conf.check_tool('g++')
    if not conf.env['CXX']:
        fatal('g++ not found')

    conf.env.append_value('CXXFLAGS', '-Wall -Werror')

    conf.check_cfg(atleast_pkgconfig_version='0.0.0')
    conf.check_cfg(package='libconfig++', uselib_store='LIBCONFIGXX', args='--cflags --libs')
    conf.check_cfg(package='glibmm-2.4', uselib_store='GLIBMM', args='--cflags --libs')
    conf.check_cfg(package='libiqxmlrpc', uselib_store='IQXMLRPC', args='--cflags --libs')

    # for unit tests
    conf.check_cfg(package='cppunit', uselib_store='CPPUNIT', args='--cflags --libs')
    if 'LIB_CPPUNIT' in conf.env:
        Utils.pprint('CYAN', "To run unit tests use 'waf check'")
    else:
        print "cppunit not found, no unit tests"

def build(bld):
    subdirs = 'src'
    if bld.env['LIB_CPPUNIT']:
        subdirs += ' tests'
    bld.add_subdirs(subdirs)

def shutdown():
    # Unit tests are run when "check" target is used
    ut = UnitTest.unit_test()
    ut.change_to_testfile_dir = True
    ut.run()
    ut.print_results()
