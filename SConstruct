# vim:ft=python

# Setup build dir
VariantDir('build', 'src', duplicate=0)

# Add --prefix option
AddOption('--prefix',
    dest='prefix',
    type='string',
    nargs=1,
    action='store',
    metavar='DIR',
    help='installation prefix',
    default='/usr/local')

env = Environment(PREFIX = GetOption('prefix'))

env.ParseConfig("pkg-config libconfig++ --cflags --libs")
env.ParseConfig("pkg-config libiqxmlrpc --cflags --libs")
env.ParseConfig("pkg-config glibmm-2.4 --cflags --libs")

#############################################
### libraries ###############################
#############################################

libecru = SharedLibrary('ecru', ['build/ecru.cc', 'build/Template.cc', 'build/Hook.cc'])

liblivejournalxx = env.SharedLibrary('livejournalxx', ['build/LiveJournal.cc', 
            'build/Config.cc', 
            'build/Event.cc'])

env.Append(LIBS=['ecru'], LIBPATH='.', CCFLAGS='-Wall -Werror')

#############################################
### apps ####################################
#############################################

ecruconfig = env.Program('ecru-config', ['build/ecru-config.cc'], LIBS=['livejournalxx', 'ecru'], LIBPATH='.')
ecrupost = env.Program('ecru-post', ['build/ecru-post.cc'], LIBS=['livejournalxx', 'ecru'], LIBPATH='.')
ecrulist = env.Program('ecru-list', ['build/ecru-list.cc'], LIBS=['livejournalxx', 'ecru'], LIBPATH='.')
ecruinfo = env.Program('ecru-info', ['build/ecru-info.cc'], LIBS=['livejournalxx', 'ecru'], LIBPATH='.')
ecrudelete = env.Program('ecru-delete', ['build/ecru-delete.cc'], LIBS=['livejournalxx', 'ecru'], LIBPATH='.')
ecruedit = env.Program('ecru-edit', ['build/ecru-edit.cc'], LIBS=['livejournalxx', 'ecru'], LIBPATH='.')


### UnitTest ###
# Build one or more test runners.
#program = env.Program('test', 'tests/Test.cc')
# Depend on the runner to ensure that it's built before running it - Note: using abspath.
#test_alias = Alias('test', [program], program[0].abspath)
# Simply required.  Without it, 'test' is never considered out of date.
#AlwaysBuild(test_alias)

#env.AddPostAction('foobar', program, program[0].abspath)

#############################################
### installation ############################
#############################################
lib_install = env.Install('$PREFIX/lib/', [libecru, liblivejournalxx])
app_install = env.Install('$PREFIX/bin/', [ecruconfig, ecrupost, ecrulist,
        ecruinfo, ecrudelete, ecruedit])

env.Alias('install', [lib_install, app_install])
