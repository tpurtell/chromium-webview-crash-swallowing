# Test App for Exploring Android System WebView Crash Handler Behavior

There are 3 buttons in this app.
- Hook - installs a chaining signal handler that logs a message on SIGSEGV
- WebView - instantiates a WebView so that the Android System WebView crash handlers are registered
- SIGSEGV - triggers a native crash by derefering a null pointer

On Lollipop+, this app lets you observe errors in the crash handling system of the system WebView where it swallows exceptions without passing them to application (or even bionic runtime) crash handlers.

See https://code.google.com/p/chromium/issues/detail?id=481420 for a bug about the root issue.
See https://bugzilla.xamarin.com/show_bug.cgi?id=29365 for a higher level issue caused by this problem in another android runtime.
