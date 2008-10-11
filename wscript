# vim:ft=python

from logging import fatal

from Configure import conf
import Utils, Build, misc

VERSION = '0.1.0'
APPNAME = 'ecru'

srcdir = 'src'
blddir = 'output'

Utils.waf_version(mini='1.5.0', maxi='1.5.9')

def set_options(opt):
    print '  setting the options'

def configure(conf):
    conf.check_tool('g++')
    if not conf.env['CXX']:
        fatal('g++ not found')

    conf.check_cfg(atleast_pkgconfig_version='0.0.0')
    conf.check_cfg(package='libconfig++', uselib_store='LIBCONFIGXX', args='--cflags --libs')
    conf.check_cfg(package='glibmm-2.4', uselib_store='GLIBMM', args='--cflags --libs')
    conf.check_cfg(package='libiqxmlrpc', uselib_store='IQXMLRPC', args='--cflags --libs')

def build(bld):
    obj = bld.new_task_gen('subst')
    obj.source = 'ecru_version.h.in'
    obj.target = 'ecru_version.h'
    obj.dict = {'APPNAME': APPNAME, 'APPVERSION': VERSION}

    obj = bld.new_task_gen('cxx', 'shlib')
    obj.source = '''
    ecru.cc
    Template.cc
    Hook.cc
    '''
    obj.includes = '.'
    obj.target = 'ecru'
    obj.name = 'ecru'
    obj.install_path = '${PREFIX}/lib'

    obj = bld.new_task_gen('cxx', 'shlib')
    obj.source = '''
    LiveJournal.cc
    Config.cc
    Event.cc
    '''
    obj.includes = '.'
    obj.target = 'livejournalxx'
    obj.name = 'livejournalxx'
    obj.uselib = 'LIBCONFIGXX GLIBMM IQXMLRPC'
    obj.uselib_local = 'ecru'
    obj.install_path = '${PREFIX}/lib'

    tools = ['config', 'post', 'list', 'info', 'delete', 'edit']

    for tool in tools:
        obj = bld.new_task_gen('cxx', 'program')
        obj.source = ''.join(['ecru-', tool, '.cc'])
        obj.includes = '.'
        obj.uselib_local = 'ecru livejournalxx'
        obj.target = "ecru-" + tool
        obj.install_path = '${PREFIX}/bin'

