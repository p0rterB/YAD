//
//  UnitYAD.swift
//  YADTests
//
//  Created by Developer on 07.10.2023.
//

import XCTest
@testable import YAD

final class UnitYAD: XCTestCase {
    
    fileprivate static let _dataId: [UInt8] = [0x66,0x83,0x66,0xb8,0xe2,0xd3,0xfa,0x6e,0x5f,0xa9,0x01,0xcf,0xb1,0xfd,0xf4,0x78,0x5d,0x83,0xfb,0x63]
    fileprivate static let _keyBasis: [UInt8] = [0xb8,0xec,0x73,0x2a,0xda,0x8f,0x8b,0x10,0x0f,0x61,0xed,0x04,0x63,0x90,0x2c,0x8b]
    fileprivate static let _expectedKey: [UInt8] = [0xa7,0xa8,0xb0,0x5d,0x6f,0x29,0xc3,0x29,0x4d,0xe3,0x2c,0x80,0xb0,0x6d,0x93,0xb6]

    func testKeyProcessor() {
        let key = YAD.processKeyBasis(UnitYAD._keyBasis, UnitYAD._dataId)
        XCTAssertEqual(UnitYAD._expectedKey, key, "Incorrect key processor result")
    }
    
    func testDecryption() {
        guard let url = TestConstants.testBundle.url(forResource: "payload", withExtension: "enc") else {
            XCTAssertNotNil(nil, "Encoded payload asset not found")
            return
        }
        guard let decUrl = TestConstants.testBundle.url(forResource: "payload", withExtension: "dec") else {
            XCTAssertNotNil(nil, "Decoded payload asset not found")
            return
        }
        let data = (try? Data(contentsOf: url)) ?? Data()
        let decData = (try? Data(contentsOf: decUrl)) ?? Data()
        let bytes = [UInt8].init(data)
        let decBytes = [UInt8].init(decData)
        let dec = YAD.decrypt(bytes, UnitYAD._dataId, UnitYAD._keyBasis)
        XCTAssertEqual(decBytes, dec, "Incorrect decryption process")
    }
}
