/*******************************************************************
 *                     空格填充器(alignBySpace)
 * 声明：
 *  1. 软件主要是在不改变文本文件内容情况下，自动填充空格一定数量的
 *      空格，达到空格右对齐的功能；
 *  2. 本软件主要是节省个人的代码跟踪文档打空格的时间；
 *
 *                             2015-8-3 晴 深圳 南山平山村 曾剑锋
 ******************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main ( int argc, char ** argv ) {

    int    ret                      = 0;
    int    column                   = 0;
    char  *tempFile                 = "generateFile.txt";
    char  *readBuffer               = NULL;
    size_t bufferSize               = 512;
    int    readBufferIndex          = 0;
    int    writeBufferIndex         = 0;
    char   writeBuffer[bufferSize];

    if ( ( argc < 2 ) || ( argc > 3 ) ) {
        printf ( "USAGE:\n" );
        printf ( "    alignBySpace <file> [column].\n" );
        return -1;
    }

    if ( argc == 2 ) {
        printf ( "\nUse default columns: 80. \n\n" );
        column = 80;
    } else 
        column = atoi ( argv[2] );

    FILE *readfile  = fopen ( argv[1], "r" );                         // only read
    FILE *writefile = fopen ( tempFile, "w" );                        // only write

    /**
     * man datasheet:
     *    If *lineptr is NULL, then getline() will allocate a buffer for storing the line, 
     *    which should be freed by the user program. (In this case, the value in *n is ignored.)
     */
    while ( ( ret = getline( &readBuffer, &bufferSize, readfile ) ) != -1 ) {

        readBufferIndex  = 0;
        writeBufferIndex = 0;

        while ( readBufferIndex < ret ) {
            if ( readBuffer [ readBufferIndex ] != '\t' )             // kick out '\t'
                writeBuffer [ writeBufferIndex++ ] = readBuffer [ readBufferIndex++ ];
            else {
                memset ( writeBuffer + writeBufferIndex, ' ', 4 );    // every tab key was 4 space
                writeBufferIndex += 4;    
                readBufferIndex++;
            }
        }

        writeBufferIndex--;                                           // back to real index

        if ( ( column - writeBufferIndex ) > 0 ) {                    // may be real column longer than we set
            // '\n' at end of a line, need to kick it out
            memset ( writeBuffer + writeBufferIndex, ' ', column - writeBufferIndex );
            writeBuffer [ column ] = '\n';                            // for end of a line 
            writeBuffer [ column + 1 ] = '\0';                        // for end of a string
        } else 
            writeBuffer [ writeBufferIndex + 1 ] = '\0';              // make sure end of a string

        fputs ( writeBuffer, writefile );                             // write to file 

        bzero ( readBuffer,  bufferSize );                            // clean buffer
        bzero ( writeBuffer, bufferSize );

    }

    free ( readBuffer );                                              // referrence getline()

    fclose ( readfile );
    fclose ( writefile );

    remove ( argv[1] );                                               // delete source file
    rename ( tempFile, argv[1] );                                     // tempfile rename to source file
}
