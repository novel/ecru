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
ecruconfig = env.Program('ecru-config', ['src/ecru-config.cc']) #, LIBS='livejournalxx', LIBPATH='.')
ecrupost = env.Program('ecru-post', ['src/ecru-post.cc']) #, LIBS='livejournalxx', LIBPATH='.')
ecrulist = env.Program('ecru-list', ['src/ecru-list.cc']) #, LIBS='livejournalxx', LIBPATH='.')
ecruinfo = env.Program('ecru-info', ['src/ecru-info.cc'])
ecrudelete = env.Program('ecru-delete', ['src/ecru-delete.cc'])

#env.Install('/usr/local/lib', livejournalxx)
#env.Install('/usr/local/bin', [ecruconfig, ecrupost, ecrulist])
#env.Alias('install', '/usr/local/lib')
#env.Alias('install', '/usr/local/bin')
