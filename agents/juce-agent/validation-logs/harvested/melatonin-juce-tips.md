[Skip to content](https://melatonin.dev/blog/big-list-of-juce-tips-and-tricks/#wp--skip-link--target)

![](https://melatonin.dev/wp-content/uploads/2024/07/tips-1.jpg)

# The big list of JUCE tips and tricks (from n00b to pro)

Here are 33 things I wish were clearly labelled “on the box” when starting out with JUCE. I started this blog post years ago when I started learning and updated it along the way.

There’s a _lot_ of helpful info out there scattered all over the [JUCE forum](https://forum.juce.com/), the [Audio Programmer Discord](https://discord.gg/aaP8pX8Tek) and beyond.

It can be hard to know how to prioritize information when getting started. What is critical? What is nice to know? What are the agreed upon best practices vs. just someone’s esoteric way of doing things?

This assumes you have some basics covered, including:

- Basic C++ principles, such as “don’t profile performance in Debug mode.”
- Basic audio principles, such as “no memory allocation in processBlock.”

Here they are, ordered from n00b to Pro:

## The JUCE forum is key

The JUCE docs can be light on examples and gotchas. Historically, the community hasn’t been able to contribute to these docs, so they sometimes aren’t beginner friendly.

Instead, cozy up to the [forum](http://forum.juce.com/) and expect to spend hours there. It has community examples of pretty much _everything_ you want to do and 95% of problems you’ll run into getting started. The search is your new best friend.

The [tutorials](https://juce.com/learn/tutorials) are very good too. They describe mental models and concepts the docs gloss over.

## Build and run the Standalone to save time

The standalone is the fastest plugin format to iterate on, in part because it doesn’t have to be started up within a host.

Even if you don’t plan on delivering a standalone, it’s trivial to add a `Standalone` JUCE target. In CMake, it’s as easy as adding it to `FORMATS` (see [pamplejuce](http://github.com/sudara/pamplejuce) as an example). Building this doesn’t require any plugin APIs and is the quickest way to compile and iterate, especially when working on UI.

Bonus tip: To ensure your audio/midi device info and plugin state are saved, be sure to _gracefully_ quit the standalone (close the plugin window, don’t hit stop in the DAW). Thanks [The Him](http://dsp.thehim.com/)!

## Save your AudioPluginHost setup

If you are using JUCE’s AudioPluginHost as a test host (like I do!), you can save your “patch” with everything all wired up and all your parameters in place.

`File > Save` the AudioPluginHost settings as a `.filtergraph`. When your IDE loads the host next time, it will autoload the last saved “patch”.

Bonus points: add an EQ and other metering you might use regularly to the chain.

## `DBG` _will_ add dropouts and glitches in Debug

`DBG` works with strings and [allocates](https://forum.juce.com/t/anouncing-pluginval-an-open-source-cross-platform-plugin-validation-tool/27710/119?u=sudara). It’s likely you’ll hear issues if you fire too many of them in the audio thread.

Can’t have your cake and eat it too!

## New to Xcode? Turn on the console

If you are in Xcode wondering where your `DBG` output is:

`View > Debug Area > Activate Console`

## `DBG` in your DAW

There are times when you need to debug in a DAW context. You won’t be able to see output from `DBG` unless your IDE attaches to your DAW.

In Xcode, “Debug Executable” needs to be selected.

![](https://melatonin.dev/wp-content/uploads/2021/10/Screenshot-2021-10-21-CleanShot-CleanShot@2x.png)Check that box

In Clion, you can add a custom “configuration” which points to your DAW.

![](https://melatonin.dev/wp-content/uploads/2021/10/Screenshot-2021-10-21-RunDebug-Configurations-CLion-2.png)

Debugging in the DAW on Apple Silicon is now more difficult. It requires disabling SIP and [manually attaching to a process](https://forum.juce.com/t/is-there-any-way-to-debug-plugin-in-logic-pro-on-apple-m1/50576/6?u=sudara) called `AUHostingServiceXPC_arrow`. This process name [may change in the future](https://forum.juce.com/t/issues-with-m1/43476/70?u=sudara).

## AU not showing up in MacOS?

AU plugins _only_ show up in DAWs when they are in one the subfolders (`Components`, `VST3`, etc) of `/Library/Audio/Plugins` and `~/Library/Audio/Plugins`. They **must** be located there to be recognized.

If they still aren’t scanning, give the AU registrar that manages them a kick:

```
killall -9 AudioComponentRegistrar
```

[Read More](https://forum.juce.com/t/installing-new-audio-units-requires-restart-in-high-sierra/26753/45).

Bonus tip: [Anthony Nichols says](https://forum.juce.com/t/the-big-list-of-juce-tips-and-tricks-from-n00b-to-pro/62193/7?u=sudara) “set your version number to 0 for debug builds and your plugin will be auto scanned every time.”

## AUv3 won’t be registered on macOS until you run the standalone

> The AUv3 gets registered when you use the **Finder to launch** the standalone app which contains the AUv3 appex bundle (i.e. double-click on the standalone app’s icon in the Finder).
>
> [Fabian](https://forum.juce.com/t/auv3-plug-in-not-seen-by-daw/26732/2?u=sudara)

## Make sure you are _actually_ running the latest version of your plugin

Before you spend hours trying to figure out why that latest fix or feature isn’t showing up: Check if your plugin _actually_ got updated.

This can happen for myriad reasons, including:

1. You are using CMAKE but didn’t tell JUCE `COPY_PLUGIN_AFTER_BUILD TRUE` in `juce_add_plugin`
2. You might have your DAW open with the synth open while you are building a new version, so the copy fails.
3. On some platforms (windows?) you might not have correct permissions for the target folder, so the copy might be failing.

What I do: stick a JUCE `label` somewhere with the timestamp and build type:

```
label.setText (__DATE__ " " __TIME__ " " CMAKE_BUILD_TYPE, dontSendNotification);
```

Including the build type requires exposing the definition via `target_compile_definitions` in your CMakelists.txt:

```
CMAKE_BUILD_TYPE="${CMAKE_BUILD_TYPE}"
```

## Assume your UI will be the most time consuming thing

This is more of an expectation setter than a practical tip. The hard part of building a real plugin is dealing with UI.

[Use melatonin inspector](https://github.com/sudara/melatonin_inspector) to inspect your component hierarchy and positioning. Or [pluginGUIMagic](https://foleysfinest.com/developer/pluginguimagic/) to handle all UI for you.

If your goal is to create a professional looking product, this is where most of your invested time will be.

## Use CLion for consistency in dev across platforms

Not a sponsor! But [CLion](https://www.jetbrains.com/clion/) is my best JUCE friend for these reasons:

1. Gives me a consistent dev experience on both macOS and Windows
2. Syncs IDE settings between my computers and platforms
3. Has built in CMake support that makes life easy, picks up changes to CMakeLists.txt
4. Has built in [Catch2 support and UI for tests](https://melatonin.dev/blog/when-to-write-tests-for-dsp-code/)

It’s generally a nicer experience than trying to juggle Xcode on Mac and VS Code on windows!

## LookAndFeels come _before_ components that use them in member list

The order matters, specifically on destruction.

Members are deleted in the reverse order they are listed in the class. So, you want the look and feel to be deleted _after_ the component using them. In this case, `customLookAndFeel` should be declared before myOtherComponent assuming the latter uses the look and feel.

```cpp

```

## Pixels in JUCE are logical

When specifying heights and widths in JUCE, you are specifying the number of [logical pixels](https://blog.specctr.com/pixels-physical-vs-logical-c84710199d62).

They are device and resolution independent. Don’t get them confused with the number of physical pixels in your screen!

## TargetConditionals.h file not found on macOS

This can happen after updating Xcode on a CMake project. To resolve, `rm -rf` your build directory completely.

```
  fatal error: 'TargetConditionals.h' file not found

    #include <TargetConditionals.h> // (needed to find out what platform we're using)
```

## Enable repaint debugging

This macro will flashes any repainted area with random colors.

This is a good first line of defense way to [understand jank/glitch problems](https://melatonin.dev/blog/dealing-with-jank-in-juce/) with component painting. However, it’s only so helpful…

(Put `JUCE_ENABLE_REPAINT_DEBUGGING=1` in `target_compile_definitions` in your CMakeLists.txt)

If you are on macOS, you might also be interested in [Quartz Debug](https://developer.apple.com/library/archive/documentation/GraphicsAnimation/Conceptual/HighResolutionOSX/Testing/Testing.html#:~:text=Sign%20in%20to%20developer.apple,package%2C%20which%20contains%20Quartz%20Debug.) which lets you do similar.

## Edit any value live

For fine tuning UI code, `JUCE_LIVE_CONSTANT` is your friend.

For example, I find it really helpful to help me tune my shadow blur radii and opacity:

```
shadows.setRadius (JUCE_LIVE_CONSTANT(blurRadius));
shadows.setColor (colors[i].withAlpha (JUCE_LIVE_CONSTANT (0.7f)));
```

On recompile, you’ll get a lil 1990s vibes UI which will let you live edit the values.

![](https://melatonin.dev/wp-content/uploads/2024/07/Sine-Machine-v14-2024-07-20.05@2x-1024x394.jpg)

## Your plugin isn’t ready to ship until it passes pluginval

If you haven’t used pluginval yet, be prepared to fix 5 bugs you didn’t know you had.

See [my post](https://melatonin.dev/blog/pluginval-is-a-plugin-devs-best-friend/) to get started.

## If your plugin UI asserts on re-open, check your listeners

If you are having trouble tracking down the culprit, look for all those `addListener` and `addChangeListener` calls and make sure you pop their `remove` counterparts in the destructor.

## Draw with floats, use int for component bounds

Components in JUCE can technically have sub-pixel position/size when actually drawn to scale, transformed, etc.

This means their position and or size may not always be exact integers.

However, component’s _bounds_ are always specified as integer-perfect pixels. [Read more](https://forum.juce.com/t/affinetransform-scale-blurry-edges/42268/2) and see my post on [component drawing](https://melatonin.dev/blog/how-juce-components-work/).

Someday we’ll get float component bounds in JUCE! I believe.

## Use Melatonin inspector to inspect your component hierarchy

If you are guessing at why your component isn’t showing up, what size it might be, etc — you are wasting your time!

I [made a tool](https://github.com/sudara/melatonin_inspector) inspired by Web Inspector / Figma that lets you quickly zoom around, inspect and play with component properties, sizes, and more.

![](https://melatonin.dev/wp-content/uploads/2024/07/213710642-389d4313-5b5a-4656-859e-ea014c859021-891x1024.jpg)

## Better JUCE type debugging in your IDE

Jim Credland [open sourced an lldb script and natvis config](https://github.com/jcredland/juce-toys) which improves usability when debugging JUCE types such as `juce::String` and `juce::Identifier`. I added some additional types like `juce::var` and `juce::ValueTree`.

I also have a [lldb script that visualizes the contents of AudioBlocks as sparklines in your IDE](https://melatonin.dev/blog/audio-sparklines/).

## Use clang-format to stop wasting time formatting code

See [my example in Pamplejuce](https://melatonin.dev/blog/big-list-of-juce-tips-and-tricks/#%20https://forum.juce.com/t/automatic-juce-like-code-formatting-with-clang-format/31624/20%20---%20AccessModifierOffset:%20-4%20AlignAfterOpenBracket:%20DontAlign%20AlignConsecutiveAssignments:%20false%20AlignConsecutiveDeclarations:%20false%20AlignEscapedNewlines:%20Left%20AlignOperands:%20Align%20AlignTrailingComments:%20false%20AllowAllParametersOfDeclarationOnNextLine:%20false%20AllowShortBlocksOnASingleLine:%20Never%20AllowShortCaseLabelsOnASingleLine:%20false%20AllowShortFunctionsOnASingleLine:%20All%20AllowShortIfStatementsOnASingleLine:%20Never%20AllowShortLambdasOnASingleLine:%20All%20AllowShortLoopsOnASingleLine:%20false%20AlwaysBreakAfterDefinitionReturnType:%20None%20AlwaysBreakAfterReturnType:%20None%20AlwaysBreakBeforeMultilineStrings:%20false%20AlwaysBreakTemplateDeclarations:%20Yes%20BinPackArguments:%20false%20BinPackParameters:%20false%20BreakAfterJavaFieldAnnotations:%20false%20BreakBeforeBinaryOperators:%20NonAssignment%20BreakBeforeBraces:%20Custom%20BraceWrapping:%20#%20Allman%20except%20for%20lambdas%20%20%20AfterClass:%20true%20%20%20AfterCaseLabel:%20true%20%20%20AfterFunction:%20true%20%20%20AfterNamespace:%20true%20%20%20AfterStruct:%20true%20%20%20BeforeElse:%20true%20%20%20AfterControlStatement:%20Always%20%20%20BeforeLambdaBody:%20false%20BreakBeforeTernaryOperators:%20true%20BreakConstructorInitializersBeforeComma:%20false%20BreakStringLiterals:%20false%20ColumnLimit:%200%20ConstructorInitializerAllOnOneLineOrOnePerLine:%20true%20ConstructorInitializerIndentWidth:%204%20ContinuationIndentWidth:%204%20Cpp11BracedListStyle:%20false%20DerivePointerAlignment:%20false%20DisableFormat:%20false%20ExperimentalAutoDetectBinPacking:%20false%20IndentCaseLabels:%20true%20IndentPPDirectives:%20BeforeHash%20IndentWidth:%204%20IndentWrappedFunctionNames:%20true%20KeepEmptyLinesAtTheStartOfBlocks:%20false%20Language:%20Cpp%20MaxEmptyLinesToKeep:%201%20FixNamespaceComments:%20false%20NamespaceIndentation:%20All%20PointerAlignment:%20Left%20ReflowComments:%20false%20SortIncludes:%20true%20SpaceAfterCStyleCast:%20true%20SpaceAfterLogicalNot:%20false%20SpaceBeforeAssignmentOperators:%20true%20SpaceBeforeCpp11BracedList:%20true%20SpaceBeforeParens:%20NonEmptyParentheses%20SpaceInEmptyParentheses:%20false%20SpaceBeforeInheritanceColon:%20true%20SpacesInAngles:%20false%20SpacesInCStyleCastParentheses:%20false%20SpacesInContainerLiterals:%20true%20SpacesInParentheses:%20false%20SpacesInSquareBrackets:%20false%20Standard:%20%22c++20%22%20TabWidth:%204%20UseTab:%20Never%20UseCRLF:%20false%20---%20Language:%20ObjC%20BasedOnStyle:%20Chromium%20BreakBeforeBraces:%20Allman%20ColumnLimit:%200%20IndentWidth:%204%20KeepEmptyLinesAtTheStartOfBlocks:%20false%20ObjCSpaceAfterProperty:%20true%20ObjCSpaceBeforeProtocolList:%20true%20PointerAlignment:%20Left%20SpacesBeforeTrailingComments:%201%20TabWidth:%204%20UseTab:%20Never) which is derived from [Adam Wilson’s in this post](https://forum.juce.com/t/automatic-juce-like-code-formatting-with-clang-format/31624/20?u=sudara). Read how to set up [clang-format](https://www.jetbrains.com/help/clion/clangformat-as-alternative-formatter.html) in CLion.

## When you hit the JUCE leak detector, press `continue` to find _all_ the leaks

```
*** Leaked objects detected: 18 instance(s) of class AudioProcessorParameterWithID
JUCE Assertion failure in juce_LeakedObjectDetector.h:92
```

You hit that first `jassert` and think: “why am I randomly leaking this one object?”

Often there are more object types leaking. For example, the first leaked objects reported might be members of a later (or the last) leaked object.

So keep hitting `continue` in your debugger to let the program keep going. [Hat tip](https://forum.juce.com/t/debugging-leaked-objects/22223/9).

## Get comfy building your own widgets

If you have a clear picture how your UI should look and behave, in many cases it will take less time to “steal” from the stock widgets (i.e. read the code, grab relevant parts) to build exactly what you need than it is to contort the stock widgets to your will.

The JUCE widgets have a lot of legacy cruft. The widget library hasn’t received much maintenance or attention over the years. It can be tough to create a specific modern UI. It can’t do shadows properly (but [Melatonin Blur](http://github.com/sudara/melatonin_blur) can!) It wasn’t build for rounded corners.

Your desired customizations _could_ be supported out of the box (for example via `setColour` or `setBorder`). But my experience is that endless edge cases will drive you to stop bothering with customization.

Start simple: If you want to show some text and don’t need editing, don’t use a `Label`. Instead use a very simple `g.drawText` call inside of a `paint`, etc.

An exception to this rule might be the `TextEditor` — this has very sophisticated multi-line text rendering and selection logic. Not a fun wheel to reinvent.

## Don’t bother writing LookAndFeels for your custom widgets

`LookAndFeels` exist so that JUCE can offers customizable widgets. It enables many different kinds of presentation for a “widget” (such as a Label). This is useful strategy for a framework.

But you aren’t making a framework. If you are, it’s probably just for internal use. So you don’t need to provide an elaborate customization API for your “widget”.

Start simple. Keep all your drawing stuff in `paint`. You’ll thank yourself later.

## Use a better macro for variable debugging

I wrote this little helper which I find nicer than `DBG` because you can quickly inspect multiple variables:

```cpp
/* D is a little helper to print variables
*
* D(myFloat); // myFloat: 1.89
* D(myFloat, myBoolean) // myFloat: 1.89, myBoolean: 0
*/

#if (DEBUG)
    #define D(...) makePrintable (#__VA_ARGS__, __VA_ARGS__)

    template <typename Arg1>
    void makePrintable (const char* name, Arg1&& arg1)
    {
        std::cout << name << ": " << arg1 << std::endl;
    }

    template <typename Arg1, typename... Args>
    void makePrintable (const char* names, Arg1&& arg1, Args&&... args)
    {
        const char* comma = std::strchr (names + 1, ',');
        std::cout.write (names, comma - names) << ": " << arg1;
        makePrintable (comma, args...);
    }
#else
    #define D(...) // strings allocate, so no-op in Release
#endif
```

## Statically link the Windows runtime

This should probably be the default in JUCE again. It was changed to dynamic [because of an issue](https://forum.juce.com/t/important-breaking-change-juce-will-now-use-dynamic-linking-for-the-windows-runtime-by-default/25276) long [since fixed](https://blogs.windows.com/windows-insider/2019/01/09/announcing-windows-10-insider-preview-build-18312/). Dynamic [can cause problems](https://forum.juce.com/t/upgrading-visual-studio-2022-to-v17-10-x-broke-my-plug-in/61839), so ensure static in CMake with:

```
if (WIN32)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" CACHE INTERNAL "")
endif ()
```

Don’t be scared off by the “Debug” present in the snippet, [it snippet selects all configurations, regardless of Debug information](https://cmake.org/cmake/help/latest/variable/CMAKE_MSVC_RUNTIME_LIBRARY.html).

You can inspect what dlls your binary dynamically links to with the developer command prompt:

```
dumpbin /dependents <your binary path>
```

## For best performance, treat drawing like you would treat the audio thread

This just means it’s best practice to avoid allocation in paint calls when possible. It’s not always possible. I suggest having `juce::Path` or `juce::Image` or `juce::ColourGradient` objects as member variables so they are allocated once on open, not re-allocated in your paint calls.

If you really wanna dig deep into inspecting component performance, see [my module that helps you do just that](http://github.com/sudara/melatonin_perfetto).

## Beware dereferencing tons of atomics per buffer

If you are a calling a bunch of things like `gain->load()` per-sample in a tight loop in `processBlock` you will pay a performance penalty, [see this link](https://forum.juce.com/t/massive-cpu-increase-on-latest-version-of-juce/44214/3).

This only becomes a problem if you are effectively dereferencing atomics hundreds of times in an inner loop. Otherwise don’t worry about it.

## You can suspend audio processing

Call `suspendProcessing(true)` when you need to do some heavy lifting (like heavy duty preset switching, convolution reallocation) and your callbacks will return silence. [Thanks Kunz](https://forum.juce.com/t/parameter-changes-thread-safety/50098/5?u=sudara).

## Avoid static memory and singletons in plugins

`static` memory can create issues when there are multiple instances of your plugin in a DAW. This is because they _could_ be run within the same process. Or they could be in _different_ processes. You can’t know. You won’t know. It depends on the DAW.

Singletons are static memory too, so also should also be avoided in plugins. In standalone apps, they are fine. In plugins, the process/thread ambiguity makes it hard to reason about how many copies of the singleton you might have. For example, DAWs can decide to sandbox plugins into their own process (for example in Logic on an M1 or AUv3 generally), in which case you would have one singleton for each instance of your plugin.

`thread_local` won’t help you either, at least not for audio effects. Some hosts may run an entire channel’s signal chain on one thread. People might have two instances of your plugin on the channel.

Personally, almost every time I tried to use a singleton it was the wrong tool for the job, or I ran into issues.

The solution is boring — manually pass state/data structures down through your components, and to organize that data hierarchy so that classes get the minimum amount of state they need to function.

Don’t take my word for it, take [Dave Rowland’s](https://forum.juce.com/t/singleton-instance-shared-between-plugin-instances/39867/4?u=sudara).

## `parameterChanged` can happen on the audio thread

This one is important. Most people learn the hard way from their app having trouble as it’s unfortunately not properly documented.

Only do trivial things in these parameter callbacks. Yes, both `parameterChanged` and `parameterValueChanged`. Treat them like you are on the audio thread. Because they can be called from _any_ thread, including the message thread or the audio thread. It depends on the DAW.

[Avoid calling](https://forum.juce.com/t/should-i-use-sliderlistener-for-component-updates-and-valuetree-listener-for-audio-thread-updates/34806/12?u=sudara)`repaint` and avoid doing any heavy lifting for your audio stuff.

You also [might want to avoid](https://forum.juce.com/t/asyncupdater-for-audioprocessorparameter-sync/16206) [Async updater](https://forum.juce.com/t/bug-report-asyncupdater-for-real-time-use-in-parameterattachment/50645/3?u=sudara) and `Component::postCommandMessage` as posting to the message thread technically isn’t lock free. It’s still the default when using the apvts, so it seems functional enough, but I would vote to stay away from it.

There are a few popular patterns in the community. One is to use a `juce::Timer` (they are cheap and fast, you can easily have hundreds of them) to check for a boolean.

```cpp
class MyComponent : public juce::Component, juce::Timer, juce::AudioProcessorParameter::Listener
{
public:
    MyComponent()
    {
        startTimerHz (60);
    }

    void timerCallback() override
    {
        if (recalculate)
            repaint();
    }

    void paint (juce::Graphics& g) override
    {
        // paint here
    }

    void parameterValueChanged (int /*parameterIndex*/, float /*newValue*/) override
    {
        // this boolean will be picked up on next timer run
        if (!recalculate)
        {
            recalculate = true;
        }
    }

private:
    std::atomic<bool> recalculate = false;
};
```

On JUCE 7.0.6 and above you can use `VBlankAttachment` instead of the timer to ensure your component is repainted on the very next frame. That’s what I do.

## Leave your tip in the comments

And/or check out the [JUCE forum thread](https://forum.juce.com/t/the-big-list-of-juce-tips-and-tricks-from-n00b-to-pro/62193/16) for more tips!

* * *

Posted on

July 16, 2024

by [sudara](https://melatonin.dev/about/)

[c++](https://melatonin.dev/blog/category/c/), [juce](https://melatonin.dev/blog/category/juce/)

Last Updated

### Comments

1. ![Tristan Avatar](https://secure.gravatar.com/avatar/e6351f5a7f7eaca3100e9cbbe29933f0be9cb0d15e3e319172c275f3c42dc744?s=50&d=mm&r=g)







Tristan





[August 23, 2024](https://melatonin.dev/blog/big-list-of-juce-tips-and-tricks/#comment-12472)







Hi, thanks so much for this list!



I’m curious to hear more about your approach with “ Don’t bother writing LookAndFeels for your custom widgets”. Do you do this for widgets with attachments, like sliders, buttons, etc?



My current approach is a hybrid – I often wrap juce widgets inside one custom component to compartmentalize. But I still rely on look and feels for drawing the sliders, buttons, labels, etc. The downside is that my drawing code for a UI element is distributed across multiple look and feels and a component class.



Thanks again for sharing all this wisdom!





[Reply](https://melatonin.dev/blog/big-list-of-juce-tips-and-tricks/#comment-12472)




1. ![sudara Avatar](https://secure.gravatar.com/avatar/83c5404019d393cb3763cebd45e5f1f298f94e4359f6d8b0bb2f0086ad8ac254?s=50&d=mm&r=g)







      [sudara](https://melatonin.dev/)





      [August 23, 2024](https://melatonin.dev/blog/big-list-of-juce-tips-and-tricks/#comment-12473)







      Hey Tristan! Makes sense if you are wrapping JUCE components that you would still depend on LookAndFeels… That tip was more if you are writing a Button class from scratch or something, in which case you may as well just paint what you want vs. think things need to be abstracted out.

### Leave a Comment

Your email address will not be published.Required fields are marked \*

Comment \*

Name \*

Email \*

Website

Save my name, email, and website in this browser for the next time I comment.

Δ

### Related Posts

[![janky paint call in JUCE](https://melatonin.dev/wp-content/uploads/2022/04/jank-710x178.jpg)\\
Dealing with UI jank in the JUCE framework](https://melatonin.dev/blog/dealing-with-jank-in-juce/) [![](https://melatonin.dev/wp-content/uploads/2022/05/how-juce-components-work-710x178.jpg)\\
How JUCE Components Work](https://melatonin.dev/blog/how-juce-components-work/)

Twitter Widget Iframe