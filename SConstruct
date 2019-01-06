# Helper library for Simba SCons builds.
import os
import sys

sys.path.append( './lib' )
import revlib

VERSION_INC = 'inc/version.h'
TARGET_NAME = 'universe'

env = Environment()

if not env.GetOption( 'clean' ):
    revlib.getRevisionString( os.path.abspath( '.' ), VERSION_INC )

clean_files = []
clean_files.append( '.sconsign.dblite' )
clean_files.append( 'results.xml' )
clean_files.append( 'bin/core' )
clean_files.append( VERSION_INC )
env.Clean( 'default', clean_files )

env.ParseConfig( 'sdl-config --cflags --libs' )
env.Append( CPPFLAGS = [ '-g', '-Wall',  '-Wextra' ] )
env.Append( CPPPATH = [ 'inc' ] )
env.Append( CPPPATH = [ '/usr/include/libnoise' ] )
env.Append( LIBS = [ 'GL' ] )
env.Append( LIBS = [ 'config' ] )
env.Append( LIBS = [ 'noise' ] )

files = Glob( 'src/*.cpp' )

target_path = 'bin/%s' % TARGET_NAME
env.Program( target = target_path, source = files )
