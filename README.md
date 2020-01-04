# Literate

[Literate programming](https://en.wikipedia.org/wiki/Literate_programming) is a paradigm where the primary focus is to convey an understanding of the system to human readers rather than merely convince the computer to behave in a particular way. A literate program consists of an explanation of the code in a natural language such as English which is interspersed with the snippets of code to be executed. It captures not just what the software is doing, which one can read from the code itself, but *why* the author choose to do it that way.

## Advantages

Literate programming produces a far superior result than any other development approach that I've ever tried (and I've tried a few in my 35 years of programming). The list below outlines its main benefits:

1. **Clarity of thought.** By far the biggest boon is clarity in the design and implementation of the software. The literate approach forces you to think clearly and articulate what you are doing, constantly explaining the design and unfolding program code to an imaginary reader. Flaws in the design are identified and corrected earlier and explaining an ugly hack is more work and much less satisfying than coming up with an elegant solution. The literate paradigm produces a transformation in the state of mind of the programmer like no other that I've encountered.
2. **Actually useful documentation.** The tight coupling of *what* and *why* in a literate program results in documentation that is actually useful and in sync with the source code. This is far superior to the other two options that are normally observed in the wild: either no documentation at all (the most common) or documentation that is out of date and doesn't describes the current version of the software. It requires tremendous discipline to keep separate documentation and code in sync but literate programming makes this natural and easy.
3. **Structural freedom.** Literate programs can be written in whatever order is best for conveying an understanding of what they do and why the developers choose to do it that way, freeing programmers from the structure imposed by the compiler.
4. **Knowledge transfer.** New developers can bring themselves up to speed on a literate code base without being a distraction to the existing team. This is far more efficient than other paradigms where the only option is a combination of having the existing team walk new members through the code and have the new members laboriously reverse-engineered the code.

There are several misconceptions around literate programming. First, some are under the impression that literate programs need to be really wordy. In truth, the developer is free to write as much or as little as necessary to capture their thought process. Others are of the opinion that it is unnecessary because code should be entirely self-documenting. However, not only does this approach not capture the high-level design of the system, but people tend to forget that comments aren't about what is the code doing, which is obvious from reading it, but why the developer choose to do it that way.

## History

Donald Knuth developed literate programming back in late 70s and early 80s. A software application in his original system was written as a single *web* file and consisted of a mixture of English, TEX, and Pascal. This *web* file was then processed by two applications, *tangle* and *weave*. The *tangle* application extracted the Pascal portions and combined them into a source file that could be compiled to obtain the executable, while *weave* processed the entire *web* file into a typeset file that could be rendered in a visually appealing format. The figure below is from Knuth's original paper and depicts this workflow:

![Missing: Tangle and weave](images/TangleAndWeave.png "Tangle and weave")

This repository started as a fork of Zachary Yedidia's [*Literate*](https://github.com/zyedidia/Literate) application in which he extended and modernized Knuth's ideas through a number of new features such as writing the *web* files in [Markdown](https://daringfireball.net/projects/markdown/), adding support for languages other than Pascal, and supporting multiple input and output files.

This repository contains a complete overhaul of Yedidia's original work to fit my development workflow. See the [changes](Changes.md) document for details on what was changed and why.

## Building

## Limitations

Immature development tools.
Learning curve.
No "right" way.
Most programmers aren't writers.

## Application
