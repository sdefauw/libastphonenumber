# libastphonenumber Debian Package
To create the libastphonenumber DEB package, you need first to compile [libphonenumber](https://github.com/googlei18n/libphonenumber) to have the main dependency.

Creation of the package:
```
debuild -uc -us
```

The debian package file will be placed in the `../` directory. This can then be added to an APT repository or installed with `dpkg -i <package-file>`.