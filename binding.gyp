{
  "targets": [
    {
      "target_name": "s2n",
      "sources": [
        "src/node-s2n.cc",
        "src/node-s2nconfig.cc",
        "src/node-s2nconnection.cc"
      ],
      "include_dirs": [ "<!(node -e \"require('nan')\")", "/Users/adam/code/c/s2n/api" ],
      "libraries": [ "/Users/adam/code/c/s2n/lib/libs2n.a" ],
      "xcode_settings": {
        "MACOSX_DEPLOYMENT_TARGET":"10.10"
      }
    }
  ]
}
