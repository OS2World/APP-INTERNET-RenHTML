#include <cstring.h>
#include <conio.h>
#include <stdio.h>
#include <dir.h>
#include <alloc.h>

string Text,
FileName,PageName,DestName,
TitleName,H1Name,H2Name,H3Name,
TTB("<TITLE>"),TTE("/TITLE"),
H1B("<H1>"),H1E("</H1>"),
H2B("<H2>"),H2E("</H2>"),
H3B("<H3>"),H3E("</H3>");

int fi,li,i,ri,brackets;
char ch;

char Result[255];

long filesize(FILE *stream)
{
  long curpos, length;

  curpos = ftell(stream);
  fseek(stream, 0L, SEEK_END);
  length = ftell(stream);
  fseek(stream, curpos, SEEK_SET);
  return length;
}

// --- Functions

void KillNotAllowed (string &Text)
{
 string NotChars("<>?.\\/:*\r\n");
 int i,j;
 for (i=0;i<Text.length();++i)
  for (j=0;j<NotChars.length();++j)
   if (Text.get_at(i) == NotChars.get_at(j)) Text.put_at(i,'-');
}

void FindTitle (string SB,string SE)
{
// int fi,li,i,ri,brackets;

// char ch;

 Result [0] = 0;
 Text.set_case_sensitive (0);

 fi = -1;li = -1;
 fi = Text.find(SB);
 if (fi != -1)
 {
  fi = fi + SB.length();
  li = Text.find(SE,fi);
//  ch = Text.get_at(li);
//  if (ch != '<') --li;
//  ch = Text.get_at(li);
 }
 else return;
 if (li != -1)
 {
  if (li - fi > 240) li = fi+240;
  brackets = 0;ri = 0;

//  ch = Text.get_at(li);

  for (i=fi;i < li;++i)
  {
   if (Text.get_at(i) == '<') {++brackets;continue;};
   if (Text.get_at(i) == '>') {--brackets;if (brackets < 0) return;continue;}
   if (brackets == 0)
   {
    Result[ri++] = Text.get_at(i);
    Result[ri]   = 0;
   }
  }
 }
}

int LoadFromFile (string FileName)
{
 FILE *stream=NULL;long fs;
 char *str = NULL;

 stream = fopen(FileName.c_str(), "rt");
 if (!stream) return -1;
 fs = filesize(stream);
 if (fs < 10) {fclose(stream);return -1;};

 str = (char *) calloc(fs, sizeof(char));
 fread (str,sizeof(char),fs,stream);
 Text.resize(0);
 Text += str;

 free(str);
 fclose(stream);
 return 0;
}

void RenameHTML (void)
{
 char ch;

 int i;
 struct ffblk ffblk;
 int done;

 unsigned files_renamed = 0;

 done = findfirst("*.htm*",&ffblk,0);
// done = findfirst("f:\\zzz\\*.htm*",&ffblk,0);
 printf ("\n\r");

 while (!done)
 {
  Text.resize(0);
  FileName.resize(0);
  PageName.resize(0);
  DestName.resize(0);
  TitleName.resize(0);
  H1Name.resize(0);
  H2Name.resize(0);
  H3Name.resize(0);

//  FileName += "f:\\zzz\\";
  FileName += ffblk.ff_name;

  if (LoadFromFile (FileName) == -1)
  {
   done = findnext(&ffblk);
   continue;
  }

  if (Text.length() < 2) continue;

//  DestName += "f:\\zzz\\";

  FindTitle (TTB,TTE);
  TitleName  += Result;

  FindTitle (H1B,H1E);
  H1Name     += Result;

  FindTitle (H2B,H2E);
  H2Name     += Result;

  FindTitle (H3B,H3E);
  H3Name     += Result;

  if (TitleName.length() + H1Name.length() + H2Name.length() + H3Name.length() > 5)
  {

   if (TitleName.length() > 1) PageName += TitleName;
   if (H1Name.length() > 1)    PageName += '-'+H1Name;
   if (H2Name.length() > 1)    PageName += '-'+H2Name;
   if (H3Name.length() > 1)    PageName += '-'+H3Name;

/*
   printf ("\n\r------------------------------------------\n\r");
   printf ("Original File Name : %s\n\r",FileName.c_str());

   if (TitleName.length() > 1)
   {
    printf ("Title : %s\n\r\n\r",TitleName.c_str());
    PageName += TitleName;
   }

   if (H1Name.length() > 1)
   {
    printf ("Header 1 : %s\n\r",H1Name.c_str());
    printf ("to include press [ Enter ]\n\r");
    ch = getch();
    if (ch == '\xd') PageName += '-'+H1Name;
    else
    if (ch == 0) getch();
   }

   if (H2Name.length() > 1)
   {
    printf ("Header 2 : %s\n\r",H2Name.c_str());
    printf ("to include press [ Enter ]\n\r");
    ch = getch();
    if (ch == '\xd') PageName += '-'+H2Name;
    else
    if (ch == 0) getch();
   }

   if (H3Name.length() > 1)
   {
    printf ("Header 3 : %s\n\r",H3Name.c_str());
    printf ("to include press [ Enter ]\n\r");
    ch = getch();
    if (ch == '\xd') PageName += '-'+H3Name;
    else
    if (ch == 0) getch();
   }
*/
   if (PageName.length() > 240) PageName.resize(240);

   KillNotAllowed (PageName);
   DestName += PageName + ".hhttmmll";

//   printf ("New File Name      : %s\n\r",DestName.c_str());
//   printf ("----------------------------------------------\n\r");

   if (rename (FileName.c_str(),DestName.c_str()) == -1)
    printf ("Cannot rename \n\r'%s' to \n\r'%s' \n\r",FileName.c_str(),DestName.c_str());
   else
   ++files_renamed;
  }
  done = findnext(&ffblk);
 }
 printf ("\n\r[%d] file(s) renamed",files_renamed);
}


void main (void)
{
 RenameHTML();
}



