.. index:: if

if command
==========

Syntax
""""""


.. parsed-literal::

   if boolean then t1 t2 ... elif boolean f1 f2 ... elif boolean f1 f2 ... else e1 e2 ...

* boolean = a Boolean expression evaluated as TRUE or FALSE (see below)
* then = required word
* t1,t2,...,tN = one or more Packfall commands to execute if condition is met, each enclosed in quotes
* elif = optional word, can appear multiple times
* f1,f2,...,fN = one or more Packfall commands to execute if elif condition is met, each enclosed in quotes (optional arguments)
* else = optional argument
* e1,e2,...,eN = one or more Packfall commands to execute if no condition is met, each enclosed in quotes (optional arguments)

Examples
""""""""


.. parsed-literal::

   if "${steps} > 1000" then quit
   if "$x <= $y" then "print X is smaller = $x" else "print Y is smaller = $y"
   if "(${eng} > 0.0) \|\| ($n < 1000)" then &
     "timestep 0.005" &
   elif $n<10000 &
     "timestep 0.01" &
   else &
     "timestep 0.02" &
     "print 'Max step reached'"
   if "${eng} > ${eng_previous}" then "jump file1" else "jump file2"

Description
"""""""""""

This command provides an if-then-else capability within an input
script.  A Boolean expression is evaluted and the result is TRUE or
FALSE.  Note that as in the examples above, the expression can contain
variables, as defined by the :doc:`variable <variable>` command, which
will be evaluated as part of the expression.  Thus a user-defined
formula that reflects the current state of the simulation can be used
to issue one or more new commands.

If the result of the Boolean expression is TRUE, then one or more
commands (t1, t2, ..., tN) are executed.  If it is FALSE, then Boolean
expressions associated with successive elif keywords are evaluated
until one is found to be true, in which case its commands (f1, f2,
..., fN) are executed.  If no Boolean expression is TRUE, then the
commands associated with the else keyword, namely (e1, e2, ..., eN),
are executed.  The elif and else keywords and their associated
commands are optional.  If they aren't specified and the initial
Boolean expression is FALSE, then no commands are executed.

The syntax for Boolean expressions is described below.

Each command (t1, f1, e1, etc) can be any valid Packfall input script
command, except an :doc:`include <include>` command, which is not
allowed.  If the command is more than one word, it must enclosed in
quotes, so it will be treated as a single argument, as in the examples
above.

.. warning::

   If a command itself requires a quoted argument (e.g. a
   :doc:`print <print>` command), then double and single quotes can be used
   and nested in the usual manner, as in the examples above and below.
   See :ref:`Section\_commands 2 <cmd_2>` of the manual for
   more details on using quotes in arguments.  Only one of level of
   nesting is allowed, but that should be sufficient for most use cases.

Note that by using the line continuation character "&", the if command
can be spread across many lines, though it is still a single command:


.. parsed-literal::

   if "$a < $b" then &
     "print 'Minimum value = $a'" &
     "run 1000" &
   else &
     'print "Minimum value = $b"' &
     "minimize 0.001 0.001 1000 10000"

Note that if one of the commands to execute is :doc:`quit <quit>` (of an
invalid Packfall command such as "blah"), as in the first example above,
then executing the command will cause Packfall to halt.

Note that by jumping to a label in the same input script, the if
command can be used to break out of a loop.  See the :doc:`variable delete <variable>` command for info on how to delete the associated
loop variable, so that it can be re-used later in the input script.

Here is an example of a double loop which uses the if and
:doc:`jump <jump>` commands to break out of the inner loop when a
condition is met, then continues iterating thru the outer loop.


.. parsed-literal::

   label	    loopa
   variable    a loop 5
     label	    loopb
     variable  b loop 5
     print	    "A,B = $a,$b"
     run       10000
     if	    '$b > 2' then "print 'Jumping to another script'" "jump in.script break"
     next	    b
     jump	    in.script loopb
   label	    break
   variable    b delete

   next	    a
   jump	    in.script loopa


----------


The Boolean expressions for the if and elif keywords have a C-like
syntax.  Note that each expression is a single argument within the if
command.  Thus if you want to include spaces in the expression for
clarity, you must enclose the entire expression in quotes.

An expression is built out of numbers:


.. parsed-literal::

   0.2, 100, 1.0e20, -15.4, etc

and Boolean operators:


.. parsed-literal::

   A == B, A != B, A < B, A <= B, A > B, A >= B, A && B, A \|\| B, !A

Each A and B is a number or a variable reference like $a or ${abc},
or another Boolean expression.

If a variable is used it must produce a number when evaluated and
substituted for in the expression, else an error will be generated.

Expressions are evaluated left to right and have the usual C-style
precedence: the unary logical NOT operator "!" has the highest
precedence, the 4 relational operators "<", "<=", ">", and ">=" are
next; the two remaining relational operators "==" and "!=" are next;
then the logical AND operator "&&"; and finally the logical OR
operator "\|\|" has the lowest precedence.  Parenthesis can be used to
group one or more portions of an expression and/or enforce a different
order of evaluation than what would occur with the default precedence.

The 6 relational operators return either a 1.0 or 0.0 depending on
whether the relationship between x and y is TRUE or FALSE.  The
logical AND operator will return 1.0 if both its arguments are
non-zero, else it returns 0.0.  The logical OR operator will return
1.0 if either of its arguments is non-zero, else it returns 0.0.  The
logical NOT operator returns 1.0 if its argument is 0.0, else it
returns 0.0.

The overall Boolean expression produces a TRUE result if the result is
non-zero.  If the result is zero, the expression result is FALSE.


----------


Restrictions
""""""""""""
 none

Related commands
""""""""""""""""

:doc:`variable <variable>`, :doc:`print <print>`

**Default:** none


.. _lws: http://lammps.sandia.gov
.. _ld: Manual.html
.. _lc: Section_commands.html#comm
