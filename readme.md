# Welcome to Project Bliss: Virtual Machine

This is one of the projects under Project-Bliss, which covers
the platform-agnoistic code execution, i.e a virtual machine,
or just think of it as a interface layer that can run programs
written in a universally accepted bytecode format.

The binaries itself are compiled for different platform, but
the underlying code execution is guaranteed to be identical in
any platform.

## Where to start here?

If you are here, it's likely that you also want to understand
the code.

Start from the `main.c` file, and then go to different headers
from there.

## Confused?

That's normal, I wrote this code with certain intent in my mind,
although I tried to make it as readable as possible, even adding
helpful comments whereever possible, I will still release a OSS
guide about this soon.

## Why C? why not rust?

C is my favorite language, it's so simple and the mental model
is extremely simple too. Although we all know memory safety is
a valid concern, but this versions are more like a learning experience
to me as well.

And I will rigoursly test my VM with all edge cases possible,
and will try my best to fix as much of the 'invisible' bugs waiting
to happen.

## WARNING

DO NOT USE THIS VM IN PRODUCTION: The VM is still in it's early stage
and is absolutely NOT suited to be used in production. Also, I don't
know anyone who would ditch javascript just to write some assembly
that I made up for this VM, which one could argue is a million times
harder.

But if anyone wants to make some stupid decision, I still put this
warning out, because I am NOT RESPONSIBLE for any damages, regardless
of the nature when using this (read the license).