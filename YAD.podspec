Pod::Spec.new do |spec|
  spec.name         = "YAD"
  spec.version      = "0.1.4"
  spec.summary      = "Yet another decryption"
  spec.homepage     = "https://github.com/p0rterB/YAD"
  spec.license      = { :type => "MIT", :file => "LICENSE" }
  spec.author       = { "p0rterB" => "https://github.com/p0rterB" }
  spec.osx.deployment_target = "10.13"
  spec.ios.deployment_target = "11.0"
  spec.tvos.deployment_target = "11.0"
  spec.watchos.deployment_target = "4.0"
  spec.swift_version = "5.0"
  spec.source        = { :git => "https://github.com/p0rterB/YAD.git", :tag => "#{spec.version}" }
  spec.source_files  = "Sources/YAD/*.swift", "Sources/YADCXX/*.{cpp,h}", "Sources/YADCXX/include/*.h"
  spec.public_header_files = "Sources/YADCXX/*.h"
  spec.private_header_files = "Sources/YADCXX/include/*.h"
  spec.exclude_files = "Sources/YADCXX/YADCXX.cpp", "Sources/YADCXX/YADCXX.h" "Sources/Exclude", "Sources/Exclude/*.*"
  spec.library = 'c++'
  spec.xcconfig = {
   'CLANG_CXX_LANGUAGE_STANDARD' => 'c++20',
   'CLANG_CXX_LIBRARY' => 'libc++',
   'HEADER_SEARCH_PATHS' => '${PODS_ROOT}/YAD/Sources/YADCXX/include',
  }
  spec.framework     = "Foundation"
  spec.dependency      "CryptoSwift"
  spec.resource_bundles = {'YAD' => ['Sources/YAD/PrivacyInfo.xcprivacy']}
end
