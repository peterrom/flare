/* Copyright 2014 Peter Román.
   This file is part of Flare which is licensed under GNU GPL v3.
   See the file named LICENSE for details. */

#ifndef FORTY_INCL
#define FORTY_INCL

/* * FORmatted TYper (FORTY)

   FORTY is both a small markup language and a parser implementation
   for that language.

   The language syntax is very simple: text is marked by tags with
   braces, i.e. if you want a word to be written in bold face, you'd
   write

   : Lets add some {bold emphasis} to a word!

   For a hypothetical implementation of the tag "bold".

   Tags are defined through a tag list. Implementing the bold tag from
   above in ncurses would look something like

   : void push_bold(void *ctx) { attron(A_BOLD); }
   : void pop_bold(void *ctx) { attroff(A_BOLD); }
   :
   : void print(void *ctx, struct ui *text)
   : {
   :         char tmp;
   :         while (ui_get_c(text, &tmp))
   :                 printw("%c", tmp);
   : }
   :
   : struct forty_tag tl[] = {
   :         { "bold", push_bold, pop_bold },
   :         { NULL, NULL, NULL } // sentinel
   : };

   And then, from the ~main~ function, you'd simply

   : forty_parse(tl, NULL, print, is);

   To parse a UIO input stream.

   The parser implementation has a couple of limitations due to the
   fact that no dynamic memory allocation is performed.

   Data from the (possibly infinite) input stream is processed in a
   SCRATCH buffer, this imposes a limit on the length of the tag name.

   : "text {a-really-long-tag-name text ..."
   :       ^                      ^

   The string marked with ~^~ (inclusive) above has to fit inside a
   SCRATCH buffer (or else the tag will be ignored).

   The other limitation is due to the fact that the parser has to keep
   a stack of limited size of what tags are active (so that they can
   be properly "popped"). After you've nested a certain amount of
   tags, the parser will ignore any further tags.
*/

#include <stddef.h>
#include <stdbool.h>

struct ui;

struct forty_tag {
        const char *id;
        void (*push) (void *context);
        void (*pop) (void *context);
};

/* Interpret the data from ~s~ according to ~tl~ (tag list) and
   ~print~. The ~context~ pointer will be passed unchanged to all user
   supplied functions called from the parser. */
void forty_parse(const struct forty_tag *tl,
                 void *context,
                 void print(void *, struct ui *),
                 struct ui *is);

#endif /* FORTY_INCL */
