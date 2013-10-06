void DrawHud()
{
    // Temporary disable lighting
    glDisable( GL_LIGHTING );
    
    // Our HUD consists of a simple rectangle
    glMatrixMode( GL_PROJECTION );
    glPushMatrix();
        glLoadIdentity();
        glOrtho( -100.0f, 100.0f, -100.0f, 100.0f, -100.0f, 100.0f );

        glMatrixMode( GL_MODELVIEW );
        glPushMatrix();
            glLoadIdentity();
            glColor3f( 1.0f, 0.0f, 0.0f );
            glBegin( GL_QUADS );
                glVertex2f( -90.0f, 90.0f );
                glVertex2f( -90.0f, 40.0f );
                glVertex2f( -40.0f, 40.0f );
                glVertex2f( -40.0f, 90.0f );
            glEnd();
        glPopMatrix();
        glPushMatrix();
            glLoadIdentity();
            glTranslatef( -90.0f, -90.0f, 0.0f );
            glScalef( 0.1f, 0.1f, 0.1f );

            char *message = "Hello, World!";
            int index = 0;
            while( *( message + index++ ) != '\0' )
                glutStrokeCharacter( GLUT_STROKE_ROMAN, *( message + index -1 ));
            
        glPopMatrix();

    glMatrixMode( GL_PROJECTION );
    glPopMatrix();

    // Reenable lighting
    glEnable( GL_LIGHTING );
}