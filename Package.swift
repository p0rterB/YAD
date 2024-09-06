
// swift-tools-version: 5.9
import PackageDescription

let package = Package(
    name: "YAD",
    platforms: [
        .macOS(.v10_13), .iOS(.v11), .tvOS(.v11), .watchOS(.v4), .visionOS(.v1)
    ],
    products: [
        .library(name: "YADCXX", type: .static, targets: ["YADCXX"]),
        .library(name: "YAD", targets: ["YAD"]),
    ],
    dependencies: [
        .package(url: "https://github.com/krzyzanowskim/CryptoSwift.git", from: "1.8.0")
    ],
    targets: [
        .target(name: "YADCXX", exclude: ["YADCXX-XCode.cpp", "YADCXX-XCode-Bridging-Header.h"], cxxSettings: [.headerSearchPath("include")]),
        .target(name: "YAD", dependencies: [.product(name: "CryptoSwift", package: "CryptoSwift"), "YADCXX"], resources: [.copy("PrivacyInfo.xcprivacy")], swiftSettings: [.interoperabilityMode(.Cxx)]),
        .testTarget(name: "YADTests", dependencies: ["YAD"], exclude: ["TestConstantsXCodeEnvExt.swift"], resources: [
            .process("payload.dec"),
            .process("payload.enc"),
        ], swiftSettings: [.interoperabilityMode(.Cxx)]),
    ],
    swiftLanguageVersions: [.v5],
    cxxLanguageStandard: CXXLanguageStandard.cxx20
)
