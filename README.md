[1]: https://github.com/JLospinoso/matterbot
[2]: http://www.mattermost.org/webhooks/
[3]: https://api.slack.com/
[4]: http://webhooks.us/
[5]: https://github.com/Microsoft/cpprestsdk
[6]: https://jlospinoso.github.io/c++/web/rest/mattermost/software/developing/2016/06/14/matterbot.html
[7]: https://jlospinoso.github.io/c++/web/rest/mattermost/software/developing/ngrok/2016/10/14/matterbot-slack-ngrok.html

[matterbot][1] is a framework for making Mattermost/Slack bots.
It uses the [Webhooks][4] APIs exposed by both [Mattermost][2] and [Slack][3] to
send and receive messages. Microsoft's [C++ REST SDK][5] a.k.a. Casablanca is used
under the hood to manage the web-layer.

Getting started
==
See [my blog post][6] and the [follow up][7] about getting started with matterbot.

Building an example bot
==
Pull down matterbot from [github][1]:

```sh
git clone git@github.com:JLospinoso/matterbot.git
```

Open up Visual Studio. There are two projects in the solution:

* Matterbot is the project containing the matterbot (static) library.
* MatterbotSample is the project containing a sample bot

Both libraries require that NuGet has successfully installed the C++ REST SDK.
Right click on "References" > "Manage NuGet Packages" > "Installed" and make
sure that version 2.9.0 is correctly installed.

MatterbotSample contains one file, `main.cpp`, but it illustrates the main features
of the library.
