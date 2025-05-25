'use strict';
const MANIFEST = 'flutter-app-manifest';
const TEMP = 'flutter-temp-cache';
const CACHE_NAME = 'flutter-app-cache';

const RESOURCES = {"404.html": "967f22fc853566d859b23ee2706b7621",
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
"assets/NOTICES": "896761b34051d2491ad8708162072b87",
"assets/packages/font_awesome_flutter/lib/fonts/fa-brands-400.ttf": "c882514e8c13ad7fbb6b62f7c6730767",
"assets/packages/font_awesome_flutter/lib/fonts/fa-regular-400.ttf": "3ca5dc7621921b901d513cc1ce23788c",
"assets/packages/font_awesome_flutter/lib/fonts/fa-solid-900.ttf": "a2eb084b706ab40c90610942d98886ec",
"assets/packages/material_symbols_icons/lib/fonts/MaterialSymbolsOutlined.ttf": "1e8ca1d69f977ceb27a38ca0e90b5a37",
"assets/packages/material_symbols_icons/lib/fonts/MaterialSymbolsRounded.ttf": "3cba4ca7d87812e86e1778769ecd8059",
"assets/packages/material_symbols_icons/lib/fonts/MaterialSymbolsSharp.ttf": "3ec23c2e1c821134f8ee88ef6df5eb72",
"assets/shaders/ink_sparkle.frag": "ecc85a2e95f5e9f53123dcaf8cb9b6ce",
"canvaskit/canvaskit.js": "728b2d477d9b8c14593d4f9b82b484f3",
"canvaskit/canvaskit.js.symbols": "27361387bc24144b46a745f1afe92b50",
"canvaskit/canvaskit.wasm": "a37f2b0af4995714de856e21e882325c",
"canvaskit/chromium/canvaskit.js": "8191e843020c832c9cf8852a4b909d4c",
"canvaskit/chromium/canvaskit.js.symbols": "f7c5e5502d577306fb6d530b1864ff86",
"canvaskit/chromium/canvaskit.wasm": "c054c2c892172308ca5a0bd1d7a7754b",
"canvaskit/skwasm.js": "ea559890a088fe28b4ddf70e17e60052",
"canvaskit/skwasm.js.symbols": "9fe690d47b904d72c7d020bd303adf16",
"canvaskit/skwasm.wasm": "1c93738510f202d9ff44d36a4760126b",
"favicon.png": "e99f09d70e30c54528c3aa81d80aaa88",
"flutter.js": "83d881c1dbb6d6bcd6b42e274605b69c",
"flutter_bootstrap.js": "766d4920d253de0d5bd60119cab4e012",
"icons/Icon-192.png": "7be4fc11a7bf1929e616b54714c1687d",
"icons/Icon-512.png": "ee10b285308acce72ab318a138cf4edb",
"icons/Icon-maskable-192.png": "7be4fc11a7bf1929e616b54714c1687d",
"icons/Icon-maskable-512.png": "ee10b285308acce72ab318a138cf4edb",
"index.html": "a31984897a5fcaeba6a0cc642f85e2ee",
"/": "a31984897a5fcaeba6a0cc642f85e2ee",
"main.dart.js": "a027ab68dc5163c62fc133c52ce9ec54",
"main.dart.mjs": "fad28c0044fcdbf51607540a1b0e68a0",
"main.dart.wasm": "6ff990ab0ee94ef54b361cbe81e3397e",
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
