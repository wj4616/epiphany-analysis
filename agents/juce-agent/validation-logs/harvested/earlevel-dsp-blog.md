Typesetting math: 100%

## [Analog digital continuous discrete](https://www.earlevel.com/main/2023/01/16/digital-analog-continuous-discrete/)

Posted on [January 16, 2023](https://www.earlevel.com/main/2023/01/16/digital-analog-continuous-discrete/ "3:38 pm") by[Nigel Redmon](https://www.earlevel.com/main/author/codehead/ "View all posts by Nigel Redmon")

Digital and analog. We often think of them, mistakenly, as equivalent things in different domains. But that’s a misunderstanding, and we often use the terms incorrectly.

For instance, run a mic through a preamp and get a signal, we know that’s an analog signal. And if we run it through an ADC and store it in a computer, it’s digital. But if we encode that signal as up and down voltage fluctuations at a high rate of speed, and run it over a wire, such as the AES-EBU standard, arguments break out over whether the signal is digital or analog.

The arguments stem from the terms being misunderstood, with better terms available.

### Analog

The definition of analog is, “Something that bears an analogy to something else; something that is comparable.” The electrical signal from a mic is an analog of the compression and rarefaction in the air from the movement of air through your vocal cords. We can amplify it and it pushes a speaker that moves air and then our eardrums.

### Digital

Literally, digits are your fingers, for counting. Something represented by numbers is digital. But numbers are an abstractions. Long ago, most of the world standardized on symbols for a 10-based (for our ten fingers) system of number representation. We can say, “twenty-six, and write down “26”. Or “0x001A”. In binary computer memory, we can store the equivalent bits as approximate voltages, or save to a hard drive in a group of magnetic states. And transfer them over a serial connection as fluctuations of voltage. In a way, these fluctuations of voltage and magnetic states are analogs of the digital values we want to represent.

So, fundamentally analog signals are analogs, and digital signals are analogs. No wonder some argue that sending a digital signal over a wire is an analog signal. It is truly both. Perhaps we might not be using the best terms to describe what comes out of a mic and what comes out of a computer.

### Continuous

Continuous implies no separation of values. A mic signal simply flows and fluctuates over time. It’s not a series of stable states, it’s a continuum.

### Discrete

Digital audio data are discrete—individual samples updated at a fixed rate. But we can also have sampled analog as well, and it is also discrete. We can store each sample in a purely analog form—without intermediate conversion to a digit—in a bucket brigade device (BBD), essentially holding each analog voltage in a capacitor.

“Sampled” implies discrete, but in particular a series of discrete values over time (we’ll only discuss uniform sampling, where the timing is a fixed rate).

Digital is always discrete or sampled. A number is a symbol, and while we can have a series of symbols that change often, they can never be continuous. The analog lowpass filter in a DAC is what ultimately converts the discrete samples to a continuous signal.

Analog can be continuous or discrete (inside analog delay stomp boxes, for instance), though we almost always use analog as a synonym for continuous. But making the mistake of thinking they are the same thing is where more complex discussions go wrong.

So, back to the argument of whether a signal stored on a hard drive, or SSD, or in RAM are truly digital, yes they are. And they are analog, because numbers are ideas and we can’t store ideas, only analogs of ideas. It gets more confusing when transferring a digital signal over a wire, because we can’t compartmentalize electrical flow, it’s always continuous. (Strictly speaking, electron flow itself is discrete, but we’re not operating at that level—it would be horrendously intolerant of noise anyway to deal with discrete flow—which is why I say “electrical” and not “electrons”.) It’s a discrete digital signal, transported over a continuous flow of analog voltages.

But I wouldn’t call that an analog _signal_, it’s still a digital signal. If you amplify it and play it back, that’s when it will hit home that it’s not an analog of the _music signal_, it’s an analog of the _states_ of the digital signal.

Posted in [Digital Audio](https://www.earlevel.com/main/category/digital-audio/), [Fun Facts](https://www.earlevel.com/main/category/digital-audio/fun-facts/)\|[Leave a comment](https://www.earlevel.com/main/2023/01/16/digital-analog-continuous-discrete/#respond)

## [Inter-sample peaks](https://www.earlevel.com/main/2023/01/16/inter-sample-peaks/)

Posted on [January 16, 2023](https://www.earlevel.com/main/2023/01/16/inter-sample-peaks/ "12:53 pm") by[Nigel Redmon](https://www.earlevel.com/main/author/codehead/ "View all posts by Nigel Redmon")

We’ve seen that samples can represent a signal with greater peak amplitude than the peak sample values.

![](https://www.earlevel.com/main/wp-content/uploads/2023/01/4-2.png)

If those sample values were higher, the output would exceed “1.0”, or 0 dB full-scale—I’ll refer to these as “overs”. This brings two questions: Can DACs reproduce such a signal? And, what is the worst case peak between samples?

There is nothing inherent in analog design that ties the maximum digital input value to the maximum voltage a DAC can put out. But exactly how much higher the output can be depends on the design, and it’s certainly possible that there isn’t enough headroom to handle overs typical of the vast library of music available.

### How bad can it be?

There is no absolute limit to how high an inter-sample peak can be. Yikes.

Intuitively, we can understand that high frequencies are more of a problem with such peak than low, since a low frequency sine wave would have many samples per cycle, while high frequencies have few, leaving curves less well defined. As such, we can take the worst-case high frequency signal, which is alternating positive and negative extreme values, and introduce a discontinuity that causes and overshoot of the waveform. That’s illustrated in this article, [True Peak Detection](https://techblog.izotope.com/2015/08/24/true-peak-detection/), which includes a proof that there is no limit to the potential overshoot.

### All is not lost

First, real music doesn’t include sustained high frequencies at maximum amplitude. The unbounded test signal described was at the Nyquist frequency, a signal we’re technically not defined to handle, and relies on increased duration to increase the peak, at maximum sample values. Even dropping the frequency slightly would ruin the phase relation and produce maximum sample values far closer to the analog signal it represents.

For music, inter-sample overs are most likely on transient peaks, where it’s extremely difficult to tell whether the signal clipped. It doesn’t mean we shouldn’t strive to reproduce them without clipping, or avoid them in the music production process. And there is another way to avoid them even if your equipment can’t handle these overs gracefully—I’ll get to that in a moment.

### Avoiding overs in production

The sure place to avoid inter-sample overs is at the mastering stage (or he mix stage if there is no formal mastering). A true peak meter, which might oversample at 8x and check for overs, can let you know of any potential problem. I suspect about +3 dB is limit of what most music with overs might hit—in absence of true peak detection, you could hedge your bets by backing your mix down a couple of dB.

### Handling overs in DACs

While you’re unlikely to hear inter-sample overs in the first place, it’s possible that some equipment or software handles overs poorly—in the worst case, an oversampling converter that doesn’t handle magnitudes greater than 1.0 well numerically, for instance. However, this is unlikely with modern equipment, particularly using modern DAC chips.

Some DACs boast the ability to contend with overs in their design. Benchmark Audio claims to handle +3.5 dB, for instance. But few manufacturers list specs on their headroom.

### And finally…digital gain control

You can use digital gain control to reduce the sample magnitude before the DAC. You probably do it every day already. Do you keep your iPhone turned up all the way, or backed down a bit? Do you have a digital control knob, or remote volume control on your DAC for home listening? Computer output turned down a little from maximum output? Reducing just 2-3 dB from maximum output adds that much headroom to the DAC’s analog output for handling peaks. If you run a DAC with no digital gain control directly into a pre-amp, and control the analog level, you don’t have this protection, but more and more we control gain digitally, ahead of the conversion.

### In closing

It’s unlikely inter-sample overs have ruined any listening experiences for you—you probably haven’t even noticed them. And if you listen using digital volume at least a few dB below its maximum, it’s unlikely any music you’re listening to will cause inter-sample overs. Ultimately, inter-sample overs are important to understand, but not a major threat.

Posted in [Digital Audio](https://www.earlevel.com/main/category/digital-audio/), [Fun Facts](https://www.earlevel.com/main/category/digital-audio/fun-facts/), [Uncategorized](https://www.earlevel.com/main/category/uncategorized/)\|[Leave a comment](https://www.earlevel.com/main/2023/01/16/inter-sample-peaks/#respond)

## [Special case: perfect sine waves](https://www.earlevel.com/main/2023/01/15/special-case-perfect-sine-waves/)

Posted on [January 15, 2023](https://www.earlevel.com/main/2023/01/15/special-case-perfect-sine-waves/ "4:53 pm") by[Nigel Redmon](https://www.earlevel.com/main/author/codehead/ "View all posts by Nigel Redmon")

Let’s say we want to test the output of a DAC while it passes a pure sine wave. The “pure sine wave” requirement is a bit more difficult than it might seem. In the general case, it’s not possible to create a sine wave that is free of quantization error. You could scale the wave so that the amplitude peaks are on exact values, but other samples of the wave would fall between quantization levels and require numerical rounding. This is not an overwhelming problem, typically, because all of the error is confined to the ± 1 lsb area. For a signal at or near full scale and 24-bit, the total error is well below the noise level of any analog audio circuit playing it back. But the signal to noise ratio necessarily gets worse as the signal level is reduced, so a sine at the lowest possible levels has terrible S/N.

If we were to do similar tests with square waves (more precisely, bandlimited square waves, to satisfy the sample theorem), we could do much better—if we select frequencies with strict limitations. Specifically, by synchronizing the frequency to use periods that are multiples of the sampling period, and with the phase aligned so that the square wave transitions occur exactly half-way between samples. In order words, data patterns like 1,1,-1,-1,1,1,-1,-1… Put more generally, switching between a value of _A_ for _n_ samples, and _-A_ for _n_ samples, where _A_ is any exact bit value, and _n_ is an integer number of samples.

For example, toggling between a positive value for four samples and it’s corresponding negative value for four samples yields a perfect bandlimited square wave of frequency one-eighth of the sample rate and no DC offset—6 kHz for a 48 kHz sample rate. The bandlimited square wave would have three total harmonics below half the sample rate (a fourth would sit right on the Nyquist frequency).

### Sine waves

That should seem obvious, for square waves, but what about sines? First, let’s consider a square wave as described, but for higher frequencies, which would have fewer harmonics—if we can get to the point of a single harmonic below Nyquist, we’d have a pure sine wave! The highest frequency obtainable with the square wave method we’re using would be alternating samples, _A,-A,A,-A…_ But that would be a square wave at the Nyquist rate, equivalent to a sine wave at the Nyquist rate, and we’d expect a proper converter to wipe that out. Next up is _A,A,-A,-A…_, half Nyquist. That would indeed be a single harmonic—the fundamental at one-quarter the sample rate, 12 kHz for 48 kHz SR. And completely free of quantization. Here’s a representation of how the sine wave relates to the samples:

![](https://www.earlevel.com/main/wp-content/uploads/2023/01/4.png)

Note that once converted to analog, the resulting sine wave has inter-sample peaks greater than we’d expect from the digital values. Inter-sample peaks are always a fact of life with digital audio, I’ll publish an article on them soon. But in this case, we could change the phase and align the peaks on exact sample boundaries, resulting in peaks that are exactly represented by the sample values, while retaining the error-free quality:

![](https://www.earlevel.com/main/wp-content/uploads/2023/01/4-1.png)

Now, we’re identified one perfect sign wave, at one-quarter the sample rate. Any others? Not from square waves, because we can’t go higher, and if we go lower we introduce more harmonics. But let’s think about the sine and why we have a perfect one. The answer is that one-quarter works because it features two things we need: it produces a sine wave, and it only uses values that are an exact bit value—either an exact level of our choosing and it’s negative, or zero.

Ultimately, we have only a few choices that can produce a sine from only 0, _A_, and _-A_. In addition to one-fourth the sample rate, one-third and one-sixth satisfy the requirements:

![](https://www.earlevel.com/main/wp-content/uploads/2023/01/3.png)![](https://www.earlevel.com/main/wp-content/uploads/2023/01/6.png)

What about other integer factors? We have to skip 2 (Nyquist), we’ve seen 3, 4, and 6; 5 doesn’t work, because although it’s symmetrical, it produces two levels per side—both can’t be exact sample values. If you try anything over 6 (3 samples per half-cycle) you’ll find it provides too many sample points per half cycle to maintain a single value (±) along with zero.

So that’s it—1/3, 1/4, and 1/6. For a 48 kHz sample rate, we can have perfect 16, 12, or 8 kHz sines. The _A_ can be any value, from a single lsb to the maximum word size. But remember, positive for DACs doesn’t go quite to “1.0”, so balance the maximum negative level accordingly, and also take into account inter-sample peaks. However, it seems these special cases of sines are mainly useful only for very small amplitudes, so those two caveats can be ignored in that case. For full-scale sines and 24-bit converters, the imperfections from calculation and quantization are small, and in particular much smaller than the minimum analog circuitry noise, so we would normally calculate our sine tones with traditional sine computation (we might use a sine function, or an FFT especially for multiple harmonic tones).

### Calculating values

To calculate the exact values needed for a sine wave of amplitude ± 1.0, use sin(\\frac{2πx}{div})sin(2πxdiv), where x is the integer sample number (0, 1, 2…), and _div_ is the divisor of the sample rate (3, 4, or 6 for “perfect” sine waves). You can multiply the sample values by a constant to scale them to the desired signal size. The idea, of course, is to round them to exact samples values, symmetrically around zero. You’ll get this automatically with rounding towards zero. And don’t dither these special cases—they have no noise, so you’ll only introduce noise with dither.

This article was the result of thought and discussion [here](https://www.audiosciencereview.com/forum/index.php?threads/how-good-is-your-pcm-sine-wave-test-tone-nivens-theorem-is-your-friend.40376/).

Posted in [Digital Audio](https://www.earlevel.com/main/category/digital-audio/), [Fun Facts](https://www.earlevel.com/main/category/digital-audio/fun-facts/)\|[Leave a comment](https://www.earlevel.com/main/2023/01/15/special-case-perfect-sine-waves/#respond)

## [How many bits can you hear? video](https://www.earlevel.com/main/2022/04/10/how-many-bits-can-you-hear-video/)

Posted on [April 10, 2022](https://www.earlevel.com/main/2022/04/10/how-many-bits-can-you-hear-video/ "11:23 am") by[Nigel Redmon](https://www.earlevel.com/main/author/codehead/ "View all posts by Nigel Redmon")

[A listening test](https://www.earlevel.com/main/2021/11/04/a-listening-test/) in video form. On the plus side, it’s helpful to see the spectrum as it’s playing, and a peak meter to see how the sweeps compare to each other, and to the announcing voice. On the minus side, YouTube drops my 24-bit uncompressed source down to 16-bit. Worse, it somehow maintains the minimum 16-bit level for all subsequent sweeps—if you can hear the 16-bit, you’ll hear the ret too. So, for the quieter sweeps, please reference the audio from [A listening test](https://www.earlevel.com/main/2021/11/04/a-listening-test/).

_Tip: I’ve included the audio from [A listening test](https://www.earlevel.com/main/2021/11/04/a-listening-test/) in a player below the video. The audio is identical, but starts about two seconds later in the video. You can start the video, and as soon as the audio (or meter and spectrogram color) appears, start the audio player, and mute the video. As long as you’re within a second or so, it will match up fine with the video._

Posted in [Digital Audio](https://www.earlevel.com/main/category/digital-audio/), [Video](https://www.earlevel.com/main/category/video/)\|Tagged [bit level](https://www.earlevel.com/main/tag/bit-level/), [dynamic range](https://www.earlevel.com/main/tag/dynamic-range/), [listening test](https://www.earlevel.com/main/tag/listening-test/), [video](https://www.earlevel.com/main/tag/video/)\|[2 Comments](https://www.earlevel.com/main/2022/04/10/how-many-bits-can-you-hear-video/#comments)

## [A listening test](https://www.earlevel.com/main/2021/11/04/a-listening-test/)

Posted on [November 4, 2021](https://www.earlevel.com/main/2021/11/04/a-listening-test/ "5:21 pm") by[Nigel Redmon](https://www.earlevel.com/main/author/codehead/ "View all posts by Nigel Redmon")

Click to play in your browser, or right-click-download to play as you wish (24-bit .wav file):

[multi-level sweeps.wav](https://www.earlevel.com/main/wp-content/uploads/2021/11/multi-level-sweeps.wav)

I call this a listening test, because it involves your ears, equipment, and even environment. Any of these can influence your limits of hearing. There are practical limits to how quiet your environment is—a noisy street, nearby computer fans, air conditioning—we all strive to lower the background noise. Fortunately, electronic equipment it very low noise at affordable cost, but there is a minimum noise floor that can’t be eliminated at any cost, due to physical issues like Johnson-Nyquist noise and shot noise. You ears have limitations of the minimum energy required to deflect your eardrums, as well as your own human noise floor, which can include age and injury related issues such as ringing, in addition to the basics like breathing and blood flow.

The audio test file that repeats the same sweep signal, dropping in volume by one-bit increments. There are many type of test tones I could have used. It could have been a sinusoidal sweep or series of tones for each pass, dithered or not dithered, or noise bursts.

I chose to again create a purely digital signal (see [Perspective on dither](https://www.earlevel.com/main/2013/03/24/perspective-on-dither/)). The advantages are that it’s very easy to hear; it has higher harmonic content that sine waves, and it won’t compete directly with the noise of your electronics for your attention at low levels like noise test signal will. I think if it’s hearable by you in your environment and on your equipment, you’ll know with confidence whether you are hearing it or not. The fact it’s digital means there is no inherent distortion or aliasing—no quantization error, all changes line up exactly with the sample periods—and no need to add dither noise.

The audio file announces each sweep by effective sample size. The signal amplitude is two steps, peak to peak. That is, one lsb of the effective sample size positive, and one negative. That’s twice the minimum amplitude possible, but it’s a good choice because that matches things like the dither level for the sample size.

For instance, “5-bit” is announced, followed by a signal that is 2 steps in amplitude, peak to peak, out of 31 possible steps (there are 2^5 levels, or 2^5-1 steps). By the time it gets to “24-bit”, it’s 2 steps out of 16,777,215! A new sweep starts every five seconds. I used the Mac OS test-to-speech feature, unaltered from its default level. You may be annoyed that it keeps you from turning up the volume as it gets quieter, but that would be cheating the test anyway, and not give you the true idea of the relative levels. I could supply a file with only the sweeps, which is handy to find where your electronics noise floor overwhelms the signal, but I fear someone will damage their hearing or equipment, should something unexpected happen. Trust me, all electronics has a noise floor far above the 24-bit sweep, but it you want that minimum sweep, you can get it from the [Perspective on dither](https://www.earlevel.com/main/2013/03/24/perspective-on-dither/) article.

There is no quiz attached to this, it’s for you to explore you limits on your own. I hope this helps your perspective!

Posted in [Digital Audio](https://www.earlevel.com/main/category/digital-audio/)\|Tagged [24-bit test](https://www.earlevel.com/main/tag/24-bit-test/), [listening test](https://www.earlevel.com/main/tag/listening-test/)\|[2 Comments](https://www.earlevel.com/main/2021/11/04/a-listening-test/#comments)

## [Biquad calculator v3](https://www.earlevel.com/main/2021/09/02/biquad-calculator-v3/)

Posted on [September 2, 2021](https://www.earlevel.com/main/2021/09/02/biquad-calculator-v3/ "12:11 am") by[Nigel Redmon](https://www.earlevel.com/main/author/codehead/ "View all posts by Nigel Redmon")

The latest version of the biquad calculator. It also takes on the functionality of the [frequency response grapher](https://www.earlevel.com/main/2016/12/08/filter-frequency-response-grapher/):

|     |     |     |     |
| --- | --- | --- | --- |
| 100000

-80

-π/2

10

100

1000

10000

-80

-60

-40

-20

0

-π

-π/2

0

π/2

π

|     |     |
| --- | --- |
|  | Magnitude (dB) |
|  | Phase (radians) | |
| Type:lowpassType<br>highpassbandpassnotchallpasspeaklow shelfhigh shelf– first order –lowpasshighpassallpasslow shelfhigh shelfone-pole lpone-pole hp | Plot:linearlog 10log 2 | Max:autocentered40 dB30 dB20 dB10 dB0 dB-10 dB-20 dB-30 dB-40 dB | Range:20 dB40 dB60 dB80 dB100 dB120 dB140 dB160 dB180 dB |
| Sample rate (Hz) |  |
| Fc (Hz) |  |
| Q |  |
| Gain (dB) |  |
| _a_ coefficients (zeros) | _b_ coefficients (poles) |

**bigger:** Yes.

**more filters:** I probably won’t go deep into allpass filters, but people ask about calculating their coefficients from time to time, so here it is. And added first order filters for comparison.

**phase plot:** In earlier versions of the calculator, phase wasn’t important, we’re interested in an amplitude response and live with the phase response. But in adding the allpass filter type, phase is everything. It’s also good to know for other filter types, and for plotting arbitrary coefficients.

**frequency response grapher:** Edit or paste in coefficients and complete the edit with tab or a click outside the editing field to plot it. You can change the plot controls, but if you change a filter control then the calculator will resume as a biquad calculator.

For instance, clear the _b_ coefficients, and place this sequence into the a coefficients: 1,0,0,0,0,1. Then click on the graph or anywhere outside the edit field to graph it. That’s the response of summing a signal with a copy of it delayed by five samples, a simple FIR filter—a comb filter.

Because the calculator can also plot the response of arbitrary coefficients, the biquad calculator now displays the normalized b0 coefficient (1.0)—which you can ignore in a typical biquad implementation.

The coefficients fields accept values separated by almost anything—commas, spaces, new lines, for instance. And they ignore letters and values followed by “=”. You can use “a0 = 0.971, a1 = 0.215…”, for instance. Even scientific notation is accepted (“1.03e4”,).

Posted in [Biquads](https://www.earlevel.com/main/category/digital-audio/filters/iir-filters/biquads/), [Digital Audio](https://www.earlevel.com/main/category/digital-audio/), [Filters](https://www.earlevel.com/main/category/digital-audio/filters/), [Widgets](https://www.earlevel.com/main/category/widgets/)\|Tagged [biquad calculator](https://www.earlevel.com/main/tag/biquad-calculator/)\|[56 Comments](https://www.earlevel.com/main/2021/09/02/biquad-calculator-v3/#comments)

## [Time resolution in digital audio](https://www.earlevel.com/main/2021/04/08/time-resolution-in-digital-audio/)

Posted on [April 8, 2021](https://www.earlevel.com/main/2021/04/08/time-resolution-in-digital-audio/ "7:54 pm") by[Nigel Redmon](https://www.earlevel.com/main/author/codehead/ "View all posts by Nigel Redmon")

I’ve been involved in several discussions on timing resolution in digital audio, recently—honestly, I never knew it was a concern before. For example, in a video on MQA, the host explained that standard audio sample rates (44.1 and 48 kHz) were unable to match human perception of time resolution. He gave little detail, but the human limits for time resolution apparently came from a study how finely an orchestra conductor could discern timing events. At least, that’s the impression I got, with little said and no references given. But the main point is that there was a number—7 µs (or a range of 5-10 µs) for human timing resolution.

The argument against lower sample rates was that 44.1 kHz has a period (1/SR) of 22.68 µs, about three times larger than this supposed resolution of the ear. It’s also noted that a sample rate of 192 kHz would match the ear’s ability.

However, this is a fundamental misunderstanding of how digital audio works. Audio events don’t start on sample boundaries. You can advance a sine wave—and by extension, any audio event—in time by much smaller increments.

But certainly, since digital audio uses finite values, there must be some limitation, no? In a sense, there is, but it’s based on bit depth. This should be intuitive—if a signal gets small enough, below one bit, it won’t register in the digital domain and we cannot know what time it happened. In his excellent article, _[Time resolution of digital audio](https://troll-audio.com/articles/time-resolution-of-digital-audio)_, Mans Rullgard derives an equation for time resolution. The key finding is that it depends only on the number of bits in a sample and the audio frequencies involved, and has no connection to sample rate. And 16-bit audio yields far greater time resolution than we can resolve as humans. Please read the article for details.

### Here’s where I tell you it doesn’t matter

Put plainly:

_There is no time resolution limitation for dithered audio at any bit depth. Not due to sample rate, not due to bit depth._

The linked article shows why sample rate isn’t a factor, and that the time resolution is far better than the 7 µs claim. I’ll take that further and show you why even bit depth isn’t a factor—in _dithered_ audio.

I wrote earlier that, intuitively, we can understand that if a signal is below one bit, it won’t register, and we could not know when it happened from the signal information. Does the topic of sub-bit-level error sound familiar? Isn’t that what we solved with dither? Yes, it was…

### Proof by demonstration

We typically dither higher resolution audio when truncating to 16-bit. We do this to avoid audible quantization errors. This supposed timing resolution issue is a quantization issue—as Mans’ article points out. I assert that dither fixes this too, and it’s easy to demonstrate why.

You can do this experiment in your DAW, or an editor such as Audacity. It’s easiest to discuss the most common dither, TPDF dither, which simply adds a low, fixed level of white noise (specifically, triangular PDF, which is just random plus random—still sounds like white noise).

Take any 24-bit audio file, and dither it (TPDF) to 16-bit. In your DAW or editor, subtract one from the other. In your DAW, you can have each file on a separate track. If you did this right and didn’t change boundaries or alignment, both will be lined up exactly. Keep both track faders at 0 dB. Invert one—you probably have a “trim” plugin or something that lets you invert the signal. Now play both together. You should hear white noise. Actually, you’ll probably hear nothing at all, unless you’re monitoring very loudly. Be careful, but if you turn up you sound system you’ll hear faint white noise for the duration of the audio.

This is expected, of course—the whole idea of dither is to add noise to decorrelate quantization error from the music.

Now, think about what this means. The difference between the dithered 16-bit track and the original is white noise. If you record this white noise onto a third track via bussing, and play just the 24-bit original and the noise track, it is indistinguishable from the dithered 16-bit track. It’s basic mathematics. A – B = n, therefore B = A – n. (See, for this test to be 100% mathematically correct, you’ll need to invert the resulting noise track. But practically speaking, the noise isn’t correlated, so it will sound the same either way. But yeah, invert it if it worries you.)

Let’s run that by again. The 16-bit dithered audio is precisely a very low level (typically -93 dB full scale rms) white noise floor summed with the original 24-bit audio. If you don’t hear timing errors from the 24-bit audio, you won’t hear it from that with low-level white noise added, which means you won’t hear it from dithered 16-bit audio. Ever, under any circumstances.

Consider that the same is true even with 12-bit audio, or 8-bit audio—even 4-bit. If you prefer, though, you could say that there are terrible timing resolution errors in 4-bit audio, but all that dither noise is drowning them out!

Posted in [Digital Audio](https://www.earlevel.com/main/category/digital-audio/)\|[5 Comments](https://www.earlevel.com/main/2021/04/08/time-resolution-in-digital-audio/#comments)

## [Amplitude Modulation Principles and Interactive Widget video](https://www.earlevel.com/main/2020/10/29/amplitude-modulation-principles-and-interactive-widget-video/)

Posted on [October 29, 2020](https://www.earlevel.com/main/2020/10/29/amplitude-modulation-principles-and-interactive-widget-video/ "2:44 pm") by[Nigel Redmon](https://www.earlevel.com/main/author/codehead/ "View all posts by Nigel Redmon")

This video demonstrates important uses of AM—and serves as a demonstration of using the [AM widget](https://www.earlevel.com/main/2020/10/02/am-widget/).

Posted in [Amplitude Modulation](https://www.earlevel.com/main/category/math/amplitude-modulation/), [Digital Audio](https://www.earlevel.com/main/category/digital-audio/), [Video](https://www.earlevel.com/main/category/video/)\|[Leave a comment](https://www.earlevel.com/main/2020/10/29/amplitude-modulation-principles-and-interactive-widget-video/#respond)

## [Amplitude Modulation Deep and Fast video](https://www.earlevel.com/main/2020/10/29/amplitude-modulation-deep-and-fast-video/)

Posted on [October 29, 2020](https://www.earlevel.com/main/2020/10/29/amplitude-modulation-deep-and-fast-video/ "2:39 pm") by[Nigel Redmon](https://www.earlevel.com/main/author/codehead/ "View all posts by Nigel Redmon")

Here’s Amplitude Modulation Deep and Fast, a not too lengthy video that gets to the roots of what amplitude modulation does mathematically. I’ve referred to AM’s role in sampling theory in past articles, and its relatively simple math makes understanding sampling much easier than traditional explanations. And this will serve as the primary math basis for an upcoming video on sampling theory.

Fun fact: AM’s ability to convert a multiplication to simpler addition/subtraction was exploited in global navigation a few hundred years ago, in the couple of decades before the invention of logarithms displaced it.

Posted in [Amplitude Modulation](https://www.earlevel.com/main/category/math/amplitude-modulation/), [Video](https://www.earlevel.com/main/category/video/)\|[Leave a comment](https://www.earlevel.com/main/2020/10/29/amplitude-modulation-deep-and-fast-video/#respond)

## [AM widget](https://www.earlevel.com/main/2020/10/02/am-widget/)

Posted on [October 2, 2020](https://www.earlevel.com/main/2020/10/02/am-widget/ "4:51 pm") by[Nigel Redmon](https://www.earlevel.com/main/author/codehead/ "View all posts by Nigel Redmon")

1000

0.00

0

200

400

600

800

1000

0.00

0.20

0.40

0.60

0.80

1.00

Frequency

Hz

Amplitude

%

1

A

2

3

1

B

2

3

Experiment with amplitude modulation with the AM widget. Here’s a diagram of how it works; two sets of three summed sinusoids to simulate two signals, the two are multiplied together for amplitude modulation:

![](https://www.earlevel.com/main/wp-content/uploads/2020/10/AM_widget.png)

**Tremolo:** First, simulate a musical signal with three harmonics by setting A1 to 100 Hz at 60%, A2 to 200 Hz at 50%, A3 to 300 Hz at 40%. With all B amplitudes at 0%, the result is zero. Reveal the signal by setting B1 to 0 Hz at 100%. Now set B2 to 10 Hz, and bring up its amplitude—you’ll see the sidebands, responsible for the richness of the tremolo effect, created around each signal frequency.

**Ring modulation:** Continue with the same settings as the tremolo simulation, but remove B1 by setting its amplitude to 0%. Slide the B2 frequency up to simulated “ring” modulation (balanced modulation) by a sine wave at various frequencies. As you move up in frequency, the lower sidebands move downwards, but as they cross zero they seem to change direction. That’s because there is no practical difference between negative and positive frequencies—sinusoids look the same going in either direction. Although similar to tremolo, the original signal doesn’t appear in the output, only the sidebands, since there is no DC (0 Hz) offset.

**AM radio:** For standard AM radio, we must keep the signal positive by adding a DC bias. So we’ll need to set one of the frequencies to 0 Hz—since the cosine of 0 is 1, that gives us our bias, and we can use the other two frequencies to simulate the signal, the radio program material. Start with A1 at 0 Hz, 90%. Set A2 to 30 Hz at 50%, and A3 to 50 Hz at 40%. Now set B1 to 200 Hz, 100%, to simulate a radio transmission at a station frequency of 200 Hz (yes, very low, but we need to stay on the chart!). Try a second transmission by setting B2 to 400 Hz, 60%, and a third with B3 at 700 Hz, 80%. These would be three radio station broadcasting the same signal. You can change A2 and A3 to simulate changing broadcast material, and see the result in each “station” simultaneously. If you change the frequencies too much, you’ll see them spill into the areas of the other stations—this is why radio stations have band limits on broadcasts, so they don’t disrupt other stations.

**Digital audio:** We can simulate the spectrum of digital audio—this is great for understanding how aliasing behaves. We’ll simulate a signal with three frequencies, at a sample rate of 500 Hz, which means our highest allowable frequency is 250 Hz. First, construct the signal by setting A1 to 30 Hz at 100%, A2 to 60 Hz at 80%, and A3 to 90 Hz at 70%. Sampling modulates the signal with a pulse train (the “P” in PCM). It would take infinite cosines to simulate an ideal pulse train, but need only the first few to get the basic idea. Set B1 to 0 Hz at 50%. This is the offset that keeps the pulse train positive (alternating between 0 and 1) in the time domain. In the frequency domain, it’s the component that includes the signal in the result. Next set B2 to 500 Hz at 100%. This is the fundamental (first harmonic) of the sampling frequency. Finally, set B3 to 1000 Hz—the second harmonic. A perfect pulse train would have all integer multiples of the sampling frequency, but we have enough to fill the display.

Consider that graph for a moment, how conversion back to analog requires a lowpass filter to remove everything above 250 Hz. Now use the A3 frequency slider to move its component up through the spectrum. Notice that while it moves up, the corresponding sideband above moves down. If you raise A3 above 250 Hz, its “alias” is now moving downward in the audio band beneath 250 Hz. That’s why we need to avoid producing frequencies above half the sample rate—it results in aliasing.

Posted in [Amplitude Modulation](https://www.earlevel.com/main/category/math/amplitude-modulation/), [Widgets](https://www.earlevel.com/main/category/widgets/)\|[Leave a comment](https://www.earlevel.com/main/2020/10/02/am-widget/#respond)

- Search for:

- ### Recent Posts

  - [Analog digital continuous discrete](https://www.earlevel.com/main/2023/01/16/digital-analog-continuous-discrete/)
  - [Inter-sample peaks](https://www.earlevel.com/main/2023/01/16/inter-sample-peaks/)
  - [Special case: perfect sine waves](https://www.earlevel.com/main/2023/01/15/special-case-perfect-sine-waves/)
  - [How many bits can you hear? video](https://www.earlevel.com/main/2022/04/10/how-many-bits-can-you-hear-video/)
  - [A listening test](https://www.earlevel.com/main/2021/11/04/a-listening-test/)
  - [Biquad calculator v3](https://www.earlevel.com/main/2021/09/02/biquad-calculator-v3/)
  - [Time resolution in digital audio](https://www.earlevel.com/main/2021/04/08/time-resolution-in-digital-audio/)
  - [Amplitude Modulation Principles and Interactive Widget video](https://www.earlevel.com/main/2020/10/29/amplitude-modulation-principles-and-interactive-widget-video/)
- ### Archives


  - [January 2023](https://www.earlevel.com/main/2023/01/)
  - [April 2022](https://www.earlevel.com/main/2022/04/)
  - [November 2021](https://www.earlevel.com/main/2021/11/)
  - [September 2021](https://www.earlevel.com/main/2021/09/)
  - [April 2021](https://www.earlevel.com/main/2021/04/)
  - [October 2020](https://www.earlevel.com/main/2020/10/)
  - [January 2020](https://www.earlevel.com/main/2020/01/)
  - [April 2019](https://www.earlevel.com/main/2019/04/)
  - [September 2018](https://www.earlevel.com/main/2018/09/)
  - [August 2017](https://www.earlevel.com/main/2017/08/)
  - [May 2017](https://www.earlevel.com/main/2017/05/)
  - [January 2017](https://www.earlevel.com/main/2017/01/)
  - [December 2016](https://www.earlevel.com/main/2016/12/)
  - [September 2016](https://www.earlevel.com/main/2016/09/)
  - [February 2016](https://www.earlevel.com/main/2016/02/)
  - [September 2015](https://www.earlevel.com/main/2015/09/)
  - [January 2015](https://www.earlevel.com/main/2015/01/)
  - [March 2014](https://www.earlevel.com/main/2014/03/)
  - [January 2014](https://www.earlevel.com/main/2014/01/)
  - [October 2013](https://www.earlevel.com/main/2013/10/)
  - [June 2013](https://www.earlevel.com/main/2013/06/)
  - [May 2013](https://www.earlevel.com/main/2013/05/)
  - [April 2013](https://www.earlevel.com/main/2013/04/)
  - [March 2013](https://www.earlevel.com/main/2013/03/)
  - [February 2013](https://www.earlevel.com/main/2013/02/)
  - [December 2012](https://www.earlevel.com/main/2012/12/)
  - [November 2012](https://www.earlevel.com/main/2012/11/)
  - [May 2012](https://www.earlevel.com/main/2012/05/)
  - [March 2012](https://www.earlevel.com/main/2012/03/)
  - [January 2011](https://www.earlevel.com/main/2011/01/)
  - [December 2010](https://www.earlevel.com/main/2010/12/)
  - [November 2010](https://www.earlevel.com/main/2010/11/)
  - [July 2007](https://www.earlevel.com/main/2007/07/)
  - [March 2003](https://www.earlevel.com/main/2003/03/)
  - [February 2003](https://www.earlevel.com/main/2003/02/)
  - [August 2002](https://www.earlevel.com/main/2002/08/)
  - [January 1997](https://www.earlevel.com/main/1997/01/)
  - [November 1996](https://www.earlevel.com/main/1996/11/)
  - [October 1996](https://www.earlevel.com/main/1996/10/)
  - [August 1996](https://www.earlevel.com/main/1996/08/)
  - [July 1996](https://www.earlevel.com/main/1996/07/)
- ### Categories


  - [Digital Audio](https://www.earlevel.com/main/category/digital-audio/)(67)

    - [Aliasing](https://www.earlevel.com/main/category/digital-audio/aliasing/) (9)

    - [Convolution](https://www.earlevel.com/main/category/digital-audio/convolution/) (3)

    - [Dither](https://www.earlevel.com/main/category/digital-audio/dither-digital-audio/) (7)

    - [Filters](https://www.earlevel.com/main/category/digital-audio/filters/)(24)

      - [DC Blocker](https://www.earlevel.com/main/category/digital-audio/filters/dc-blocker/) (1)

      - [FIR Filters](https://www.earlevel.com/main/category/digital-audio/filters/fir-filters/) (7)

      - [IIR Filters](https://www.earlevel.com/main/category/digital-audio/filters/iir-filters/)(18)

        - [Biquads](https://www.earlevel.com/main/category/digital-audio/filters/iir-filters/biquads/) (12)
    - [Fourier](https://www.earlevel.com/main/category/digital-audio/fourier/)(3)

      - [FFT](https://www.earlevel.com/main/category/digital-audio/fourier/fft/) (3)
    - [Fun Facts](https://www.earlevel.com/main/category/digital-audio/fun-facts/) (3)

    - [Impulse Response](https://www.earlevel.com/main/category/digital-audio/impulse-response/) (5)

    - [Jitter](https://www.earlevel.com/main/category/digital-audio/jitter/) (2)

    - [Oscillators](https://www.earlevel.com/main/category/digital-audio/oscillators/)(12)

      - [Wavetable Oscillators](https://www.earlevel.com/main/category/digital-audio/oscillators/wavetable-oscillators/) (12)
    - [Phase](https://www.earlevel.com/main/category/digital-audio/phase/) (2)

    - [Reverb](https://www.earlevel.com/main/category/digital-audio/reverb/) (2)

    - [Sample Rate Conversion](https://www.earlevel.com/main/category/digital-audio/sample-rate-conversion/) (10)

    - [Sampling Theory](https://www.earlevel.com/main/category/digital-audio/sampling-theory/) (6)
  - [Effects](https://www.earlevel.com/main/category/effects/)(1)

    - [Guitar Amp Simulation](https://www.earlevel.com/main/category/effects/guitar-amp-simulation/) (1)
  - [Envelope Generators](https://www.earlevel.com/main/category/envelope-generators/) (5)

  - [Math](https://www.earlevel.com/main/category/math/)(5)

    - [Amplitude Modulation](https://www.earlevel.com/main/category/math/amplitude-modulation/) (3)
  - [MIDI](https://www.earlevel.com/main/category/midi/) (2)

  - [Source Code](https://www.earlevel.com/main/category/source-code/) (10)

  - [Synthesizers](https://www.earlevel.com/main/category/synthesizers/) (9)

  - [Uncategorized](https://www.earlevel.com/main/category/uncategorized/) (4)

  - [Video](https://www.earlevel.com/main/category/video/) (7)

  - [Widgets](https://www.earlevel.com/main/category/widgets/) (14)