env = Environment()
env.ParseConfig("pkg-config libconfig++ --cflags --libs")
env.ParseConfig("pkg-config libiqxmlrpc --cflags --libs")

env.Program('ecru-config', ['src/ecru-config.cc', 'src/Config.cc'])
env.Program('ecru-post', ['src/ecru-post.cc', 'src/LiveJournal.cc', 'src/Config.cc', 'src/PostInfo.cc'])
