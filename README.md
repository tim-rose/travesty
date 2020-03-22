# Travesty: Semi-random Text Generation.

Travesty is an implementation of the classic "travesty" text generator.
It loads some sample text, and then outputs characters based on the
letter-sequence frequencies of the input text.

The  letter-sequence length can be adjusted; longer sequences generate
"better" language, closer to the words of the original text.

## Installation

```bash
$ make build install
```

## Getting Sarted

```bash
$ travesty -s1 -o5 data/*.txt | fmt
Lorem ipsa quarrel. It take an omelette with the aid of teeth to skin
a day keeps those who live in glass houses shouldn’t count your
eggs. Don’t broke, don’t beatae vitae dicta sunt explicabo. Nemo
enim ad minim veniam, qui dolor sit amet, consequatur, vel eum iure
reprehenderit in a day. Every man has his tools. A drowning man is only
as strong as its weakest line with a thousand mile. God help them an
inch and fluxes, it may be employed in panendermic pipe to Muhammad
must go on. The best-laid plans go astray. The harder you play. You
can lead a base-plate than one. Two heads are better to bring on the
going. Beauty is in that ends well and a superaminative the horse to
it. Don’t grow fonder...
```

## Requirements

* [devkit](https://github.com/tim-rose/devkit)
* [libxtd](https://github.com/tim-rose/libxtd)

## License

You are licensed to use travesty under the MIT license.
See the file [LICENSE](LICENSE) for details.

## TODO

* Add tests.
