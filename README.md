<div align="center">
    <h1>SpotiHook</h1>
</div>

A Win32 module that once injected into Spotify, can download songs directly from Spotify servers regardless whether or not you're listening to them.

Most of the functions come from meik97's XSpotify project (huge shoutouts to him and also fortender and pr8x for helping), but I have tried to readapt the software in order for it to have it's own little GUI.

This project works creating a DirectX device on nVidia's hWnd and drawing the overlay there. Therefore it only works if you have nVidia GPU with nVidia GeForce Experience drivers installed.
This is because I've built this project to my personal needings, interfacing it with "advancedHook", a private project I'm still working on.

<img src="https://i.imgur.com/4wQjiJo.png">

## Libraries used
* XSpotify by meik97
* OpenSSL
* zgui by zxvnme

## Thanks to
harakirinox
