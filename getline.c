/*
getline.c

$Id: getline.c 95 2008-01-25 15:37:10Z mjs $
*/

#include <stdio.h>

#include "getline.h"
#include "printmsg.h"
#include "errdefs.h"
#include "miputil.h"

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

static char *line=NULL;
static unsigned int maxlen=0;
#define GULP 128

char *pchGetLine(FILE *fp, int raw, int *linesread)
/* reads lines from the file pointed to by fp. if raw is true
all lines are return (including those that are blank or contain
comments). if raw is false then comments are ignored, blanks lines
are skipped and white space is stripped from the beginning and end
of lines. A backslash at the end of the line escapes the linefeed
and continues the next line. The function returns a pointer
to a buffer containing the line (the buffer is overwritten
for each line). Also, the absolute number of lines read (including
comment lines, blank lines, and continued lines, is returned in 
linesread number of lines read.
*/
{
	int c, lastc;
	char *pc, *pc1;
	size_t linelen, savepos;
	int linenum = 0;
	int valid;

	if (!raw){
		valid=FALSE;
		do{
			/* skip leading white space and blank lines */
			while((c = getc(fp)) != EOF && (c == ' ' || c == '\n' || c =='\t'))
				if (c == '\n')
					++linenum;
			if (c == EOF){
				vPrintMsg(10,"getline: EOF (1)\n");
				*linesread=linenum;
				return (NULL);
			}
			/* if the line is a comment skip it */
			if(c == '!'|| c == ';' || c == ':' || c == '#'){
				vPrintMsg(10,"getline: comment\n");
				++linenum;
				while((c = getc(fp)) != EOF && c != '\n')
					;
			}else{
				ungetc(c, fp);
				valid = TRUE;
			}
		}while (!valid);
	}
	/* copy the line up to a newline into line */
	pc = line;
	linelen=1;/* start line length at 1 to account for need to have
						terminating \0 */
	lastc='\0';
	vPrintMsg(10,"getline: linenum=%d\n",linenum);
	while((c = getc(fp)) != EOF){
		if (c == '\n' && lastc != '\\'){
			vPrintMsg(10,"getline: eol %d\n",linenum);
			break;
		}
		if (c == '\n' && lastc == '\\')
			pc--;
		lastc=c;
		if(c == '!' || c == ';' || c == '#'){
			/*++linenum;*/
			vPrintMsg(10,"getline: eol comment %d\n",linenum);
			while((c = getc(fp)) != EOF && c != '\n')
				;
			break;
		}
		if (c != '\n') {
			if (++linelen > maxlen){
				savepos = (size_t)pc-(size_t)line;
				vPrintMsg(10,"getline: allocate\n");
				/* note that IrlRealloc takes care of the case where line is NULL*/
				line = (char *)pvIrlRealloc(line, (maxlen+GULP)*sizeof(char),
					"getline:line");
				maxlen += GULP;
				pc=line+savepos;
			}
			*pc++=c;
		}
	}
	if (c == EOF && pc == line){
		vPrintMsg(10,"getline: EOF (2)\n");
		*linesread = linenum;
		return(NULL);
	}

	++linenum;
	if (!raw){
		/*remove trailing white space*/
		--pc;
		while (pc > line && (*pc == ' ' || *pc == '\t'))
			--pc;
		++pc; /*adjust pc to point past the last character*/
		/* remove comments */
		for (pc1 = line; pc1 < pc && *pc1 != '!'; ++pc1)
			;
		if (*pc1 == '!')
			pc = pc1;
	}
	*pc = '\0';
	*linesread=linenum;
	return (line);
}

void vFreeLineBuf(void)
{
	IrlFree(line);
	line=NULL;
	maxlen=0;
}
