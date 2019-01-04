def getRevisionString( file, versionfile ):
    import os
    from mercurial import hg, ui
    from mercurial.node import hex

    try:
        repo = hg.repository( ui.ui(), file )
        fctx = repo.filectx( '.', 'tip' )
        hexnode = hex( fctx.node() )
        revnum = fctx.rev()

        mod = 0
        for i in range( 0, len( repo.status() ) ):
            mod = mod + len( repo.status()[i] )

        if mod>0:
            state = 'M'
        else:
            state = ''

        versionString = '%s:%s%s' % (revnum, hexnode[:12], state)
    except Exception, err:
        versionString = 'N/A'

    versiontext = []
    versiontext.append( '#ifndef VERSION_H\n' )
    versiontext.append( '#define VERSION_H\n' )
    versiontext.append( '\n' )
    versiontext.append( '#define VERSION_STRING "%s"\n' % versionString )
    versiontext.append( '\n' )
    versiontext.append( '#endif\n' )

    f = open( versionfile, 'w' )
    f.writelines( versiontext )
    f.close()
