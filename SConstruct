# vim:ft=python

env = Environment()
env.ParseConfig("pkg-config libconfig++ --cflags --libs")
env.ParseConfig("pkg-config libiqxmlrpc --cflags --libs")
env.ParseConfig("pkg-config glibmm-2.4 --cflags --libs")

env.StaticLibrary('ecru', ['src/ecru.cc', 'src/Template.cc'])

env.StaticLibrary('livejournalxx', ['src/LiveJournal.cc', 
            'src/Config.cc', 
            'src/Event.cc'])

env.Append(LIBS=['livejournalxx', 'ecru'], LIBPATH='.', CCFLAGS='-Wall -Werror')
ecruconfig = env.Program('ecru-config', ['src/ecru-config.cc'])
ecrupost = env.Program('ecru-post', ['src/ecru-post.cc'])
ecrulist = env.Program('ecru-list', ['src/ecru-list.cc'])
ecruinfo = env.Program('ecru-info', ['src/ecru-info.cc'])
ecrudelete = env.Program('ecru-delete', ['src/ecru-delete.cc'])
ecruedit = env.Program('ecru-edit', ['src/ecru-edit.cc'])

### UnitTest ###
# Build one or more test runners.
#program = env.Program('test', 'tests/Test.cc')
# Depend on the runner to ensure that it's built before running it - Note: using abspath.
#test_alias = Alias('test', [program], program[0].abspath)
# Simply required.  Without it, 'test' is never considered out of date.
#AlwaysBuild(test_alias)

#env.Install('/usr/local/lib', livejournalxx)
#env.Install('/usr/local/bin', [ecruconfig, ecrupost, ecrulist])
#env.Alias('install', '/usr/local/lib')
#env.Alias('install', '/usr/local/bin')
