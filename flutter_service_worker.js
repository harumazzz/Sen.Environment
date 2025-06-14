'use strict';
const MANIFEST = 'flutter-app-manifest';
const TEMP = 'flutter-temp-cache';
const CACHE_NAME = 'flutter-app-cache';

const RESOURCES = {"404.html": "b211b34025625fceb1ca76f91f040331",
"assets/AssetManifest.bin": "8e14493282857bb240ca922a048a7627",
"assets/AssetManifest.bin.json": "3c948cb4e53cd6f399224f7a25b71a63",
"assets/AssetManifest.json": "78ea56063be0ef2a17655e236067111c",
"assets/assets/dark/anim.webp": "cb3baa04160dc111006b82902501af3c",
"assets/assets/dark/home.webp": "8a0d246bdfd7d5782d5d3070dd750151",
"assets/assets/dark/launcher.webp": "8e3a293629778b2723489d6799121b40",
"assets/assets/dark/map.webp": "d46d9deef24f57f31a85ea2aee465f0d",
"assets/assets/dark/phone.webp": "e2266af8a812cb42844387bbf4e35fdd",
"assets/assets/demo.webp": "8827b990a3308c40f21e9b4b1095a5c2",
"assets/assets/icons/addendum.webp": "09a9d84364a8333e6ebfb715609ce812",
"assets/assets/icons/altverz.webp": "2813b291d133261c8e581cf4722e27d1",
"assets/assets/icons/fallen.webp": "972e67b3723697b3c8cd54f5780952f2",
"assets/assets/icons/reimagined.webp": "613e75c85c93d0e040fcc7886781bc06",
"assets/assets/icons/requiem.webp": "0be0bea83b40f01cfd86c7713d2eae8d",
"assets/assets/icons/resonance.webp": "096c44c1ec72f17ce156fff8d6d7a9f7",
"assets/assets/light/anim.webp": "0dd7ca07a76603b68c4819980fc3b747",
"assets/assets/light/home.webp": "e5090b10a4fba277a969f4f47e2a1201",
"assets/assets/light/launcher.webp": "222bf25bbbe639ba36aba212e4fdd472",
"assets/assets/light/map.webp": "56a4584d570b2584679685730ab258df",
"assets/assets/light/phone.webp": "3643416380a6ba4f9efd3b63ab2ff8d7",
"assets/assets/logo/android.webp": "0de3deea5040b25f13e0c6a47d82f592",
"assets/assets/logo/windows.webp": "0da58ee8ee269de164a7372a62a2d5e2",
"assets/assets/logo.webp": "9db92892259fa2e704bc8257bd07811f",
"assets/assets/phone_view.webp": "6e6e627443ec0ec78c2a801938a8df63",
"assets/assets/terminal.webp": "9a4fc496002af492747300ca63edd574",
"assets/FontManifest.json": "5618c46fd5df5c5c621a07c41adb719c",
"assets/fonts/MaterialIcons-Regular.otf": "c0ad29d56cfe3890223c02da3c6e0448",
"assets/NOTICES": "9e59acb0ca97cf0de7832c516263050c",
"assets/packages/font_awesome_flutter/lib/fonts/fa-brands-400.ttf": "f7104b0b6a7937985ecdb5ffef65bf8a",
"assets/packages/font_awesome_flutter/lib/fonts/fa-regular-400.ttf": "3ca5dc7621921b901d513cc1ce23788c",
"assets/packages/font_awesome_flutter/lib/fonts/fa-solid-900.ttf": "a2eb084b706ab40c90610942d98886ec",
"assets/packages/material_symbols_icons/lib/fonts/MaterialSymbolsOutlined.ttf": "1e8ca1d69f977ceb27a38ca0e90b5a37",
"assets/packages/material_symbols_icons/lib/fonts/MaterialSymbolsRounded.ttf": "3cba4ca7d87812e86e1778769ecd8059",
"assets/packages/material_symbols_icons/lib/fonts/MaterialSymbolsSharp.ttf": "3ec23c2e1c821134f8ee88ef6df5eb72",
"assets/shaders/ink_sparkle.frag": "ecc85a2e95f5e9f53123dcaf8cb9b6ce",
"canvaskit/canvaskit.js": "728b2d477d9b8c14593d4f9b82b484f3",
"canvaskit/canvaskit.js.symbols": "bdcd3835edf8586b6d6edfce8749fb77",
"canvaskit/canvaskit.wasm": "7a3f4ae7d65fc1de6a6e7ddd3224bc93",
"canvaskit/chromium/canvaskit.js": "8191e843020c832c9cf8852a4b909d4c",
"canvaskit/chromium/canvaskit.js.symbols": "b61b5f4673c9698029fa0a746a9ad581",
"canvaskit/chromium/canvaskit.wasm": "f504de372e31c8031018a9ec0a9ef5f0",
"canvaskit/skwasm.js": "ea559890a088fe28b4ddf70e17e60052",
"canvaskit/skwasm.js.symbols": "e72c79950c8a8483d826a7f0560573a1",
"canvaskit/skwasm.wasm": "39dd80367a4e71582d234948adc521c0",
"favicon.png": "e99f09d70e30c54528c3aa81d80aaa88",
"flutter.js": "83d881c1dbb6d6bcd6b42e274605b69c",
"flutter_bootstrap.js": "fb64e316e06ce6ce754c1d3680c43cfc",
"icons/Icon-192.png": "7be4fc11a7bf1929e616b54714c1687d",
"icons/Icon-512.png": "ee10b285308acce72ab318a138cf4edb",
"icons/Icon-maskable-192.png": "7be4fc11a7bf1929e616b54714c1687d",
"icons/Icon-maskable-512.png": "ee10b285308acce72ab318a138cf4edb",
"index.html": "a31984897a5fcaeba6a0cc642f85e2ee",
"/": "a31984897a5fcaeba6a0cc642f85e2ee",
"main.dart.js": "ad43524328db2aed90cec9ff11ff9a2b",
"main.dart.js_1.part.js": "7de000b263382f171b723f94ff8a2587",
"main.dart.js_10.part.js": "0db070535051764b018aaee8da62957a",
"main.dart.js_12.part.js": "29a8c022a6ee62c04a92a74e9055cf63",
"main.dart.js_13.part.js": "b7a762dad4afee9c3552a2d90da7da79",
"main.dart.js_14.part.js": "97064ec40922a99d2bfd9053dce36b92",
"main.dart.js_15.part.js": "400b7f8741e6f8682d58145b79056680",
"main.dart.js_16.part.js": "393fd10f058e913b469c060cdc8e60f7",
"main.dart.js_17.part.js": "c8eec936644fafd5d8718c31d61d18b7",
"main.dart.js_19.part.js": "96e9c1e25816dc80459de8dd8ff85f49",
"main.dart.js_2.part.js": "cc34aaa2ffe85cbcd2cc9918a7b97e06",
"main.dart.js_20.part.js": "020d7fe2aee82c5455941dbe4b3a20d8",
"main.dart.js_21.part.js": "e39c144317c5a7fc8420b8f1abf9dfa8",
"main.dart.js_22.part.js": "77d24fd5f256a7dbda4d0fd5cf8bb551",
"main.dart.js_23.part.js": "d57196e51ca3d8751cc198cc2867c954",
"main.dart.js_24.part.js": "325e6e45a4f786ab416958a0c8657bb3",
"main.dart.js_25.part.js": "8dfadf5ec053f413350a4c044b530d9e",
"main.dart.js_26.part.js": "23b829dcbde03b3d2f7d381572f9043c",
"main.dart.js_27.part.js": "efd58621c8c53e47820d2b10624a5137",
"main.dart.js_28.part.js": "d3b1264e705a7c256a7c0e5717e8815b",
"main.dart.js_29.part.js": "b4c7c87fa16d4881d152120b37679728",
"main.dart.js_30.part.js": "facaf8de46ebeed623df85cd06552c57",
"main.dart.js_31.part.js": "92c97a744c98c7ba465ee5fb8769d112",
"main.dart.js_32.part.js": "3882763f8e6a951cb529ac74ff3144c7",
"main.dart.js_33.part.js": "40d46ac54d3847aacc59a9c39eb76d97",
"main.dart.js_34.part.js": "32a46f21a145d4b69504a27c5fe03483",
"main.dart.js_35.part.js": "e8e35bf6eb09cd31f46b3a816b6242ad",
"main.dart.js_36.part.js": "5a5f83680d85802e955e10827044bbf3",
"main.dart.js_37.part.js": "2159ba0f4024a130ecdbc3719aa75bef",
"main.dart.js_7.part.js": "cee0fa0e4564413c6623356eb32578b6",
"main.dart.js_8.part.js": "6f9170ded1533717ba96e608ced8eb9d",
"main.dart.js_9.part.js": "1fdbedb5437ac4c6f1a7f932636cc017",
"main.dart.mjs": "ec7a5b15c7f36c8d6c6ec3fedb9a19cb",
"main.dart.wasm": "7ffe1d4d5266cabbf4d3cf2f30245889",
"manifest.json": "1046394ca1d2d7fc9beb00b62118f232",
"styles.css": "823a83dd1efc5febe894cd370fb2509d",
"version.json": "980547175e325fe622a3362b84d55b6a"};
// The application shell files that are downloaded before a service worker can
// start.
const CORE = ["main.dart.js",
"main.dart.wasm",
"main.dart.mjs",
"index.html",
"flutter_bootstrap.js",
"assets/AssetManifest.bin.json",
"assets/FontManifest.json"];

// During install, the TEMP cache is populated with the application shell files.
self.addEventListener("install", (event) => {
  self.skipWaiting();
  return event.waitUntil(
    caches.open(TEMP).then((cache) => {
      return cache.addAll(
        CORE.map((value) => new Request(value, {'cache': 'reload'})));
    })
  );
});
// During activate, the cache is populated with the temp files downloaded in
// install. If this service worker is upgrading from one with a saved
// MANIFEST, then use this to retain unchanged resource files.
self.addEventListener("activate", function(event) {
  return event.waitUntil(async function() {
    try {
      var contentCache = await caches.open(CACHE_NAME);
      var tempCache = await caches.open(TEMP);
      var manifestCache = await caches.open(MANIFEST);
      var manifest = await manifestCache.match('manifest');
      // When there is no prior manifest, clear the entire cache.
      if (!manifest) {
        await caches.delete(CACHE_NAME);
        contentCache = await caches.open(CACHE_NAME);
        for (var request of await tempCache.keys()) {
          var response = await tempCache.match(request);
          await contentCache.put(request, response);
        }
        await caches.delete(TEMP);
        // Save the manifest to make future upgrades efficient.
        await manifestCache.put('manifest', new Response(JSON.stringify(RESOURCES)));
        // Claim client to enable caching on first launch
        self.clients.claim();
        return;
      }
      var oldManifest = await manifest.json();
      var origin = self.location.origin;
      for (var request of await contentCache.keys()) {
        var key = request.url.substring(origin.length + 1);
        if (key == "") {
          key = "/";
        }
        // If a resource from the old manifest is not in the new cache, or if
        // the MD5 sum has changed, delete it. Otherwise the resource is left
        // in the cache and can be reused by the new service worker.
        if (!RESOURCES[key] || RESOURCES[key] != oldManifest[key]) {
          await contentCache.delete(request);
        }
      }
      // Populate the cache with the app shell TEMP files, potentially overwriting
      // cache files preserved above.
      for (var request of await tempCache.keys()) {
        var response = await tempCache.match(request);
        await contentCache.put(request, response);
      }
      await caches.delete(TEMP);
      // Save the manifest to make future upgrades efficient.
      await manifestCache.put('manifest', new Response(JSON.stringify(RESOURCES)));
      // Claim client to enable caching on first launch
      self.clients.claim();
      return;
    } catch (err) {
      // On an unhandled exception the state of the cache cannot be guaranteed.
      console.error('Failed to upgrade service worker: ' + err);
      await caches.delete(CACHE_NAME);
      await caches.delete(TEMP);
      await caches.delete(MANIFEST);
    }
  }());
});
// The fetch handler redirects requests for RESOURCE files to the service
// worker cache.
self.addEventListener("fetch", (event) => {
  if (event.request.method !== 'GET') {
    return;
  }
  var origin = self.location.origin;
  var key = event.request.url.substring(origin.length + 1);
  // Redirect URLs to the index.html
  if (key.indexOf('?v=') != -1) {
    key = key.split('?v=')[0];
  }
  if (event.request.url == origin || event.request.url.startsWith(origin + '/#') || key == '') {
    key = '/';
  }
  // If the URL is not the RESOURCE list then return to signal that the
  // browser should take over.
  if (!RESOURCES[key]) {
    return;
  }
  // If the URL is the index.html, perform an online-first request.
  if (key == '/') {
    return onlineFirst(event);
  }
  event.respondWith(caches.open(CACHE_NAME)
    .then((cache) =>  {
      return cache.match(event.request).then((response) => {
        // Either respond with the cached resource, or perform a fetch and
        // lazily populate the cache only if the resource was successfully fetched.
        return response || fetch(event.request).then((response) => {
          if (response && Boolean(response.ok)) {
            cache.put(event.request, response.clone());
          }
          return response;
        });
      })
    })
  );
});
self.addEventListener('message', (event) => {
  // SkipWaiting can be used to immediately activate a waiting service worker.
  // This will also require a page refresh triggered by the main worker.
  if (event.data === 'skipWaiting') {
    self.skipWaiting();
    return;
  }
  if (event.data === 'downloadOffline') {
    downloadOffline();
    return;
  }
});
// Download offline will check the RESOURCES for all files not in the cache
// and populate them.
async function downloadOffline() {
  var resources = [];
  var contentCache = await caches.open(CACHE_NAME);
  var currentContent = {};
  for (var request of await contentCache.keys()) {
    var key = request.url.substring(origin.length + 1);
    if (key == "") {
      key = "/";
    }
    currentContent[key] = true;
  }
  for (var resourceKey of Object.keys(RESOURCES)) {
    if (!currentContent[resourceKey]) {
      resources.push(resourceKey);
    }
  }
  return contentCache.addAll(resources);
}
// Attempt to download the resource online before falling back to
// the offline cache.
function onlineFirst(event) {
  return event.respondWith(
    fetch(event.request).then((response) => {
      return caches.open(CACHE_NAME).then((cache) => {
        cache.put(event.request, response.clone());
        return response;
      });
    }).catch((error) => {
      return caches.open(CACHE_NAME).then((cache) => {
        return cache.match(event.request).then((response) => {
          if (response != null) {
            return response;
          }
          throw error;
        });
      });
    })
  );
}
