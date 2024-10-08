//
//  UnitYAD.swift
//  YADTests
//
//  Created by Developer on 07.10.2023.
//

import XCTest
@testable import YAD

final class UnitYAD: XCTestCase {
    
    fileprivate static let _dataId: [UInt8] = [0x04,0x7a,0x05,0xe6,0x2e,0x72,0x52,0xda,0x89,0xd9,0x3e,0x25,0x88,0xc1,0x09,0xbc,0x27,0xe2,0x8a,0x87]
    fileprivate static let _keyBasis: [UInt8] = [0x53,0x0d,0xef,0x32,0x49,0xf2,0x81,0x26,0x50,0x19,0xd1,0x2b,0x8d,0x79,0x01,0xea]
    fileprivate static let _expectedKey: [UInt8] = [0xf5,0x63,0x53,0x6e,0x8e,0x1e,0x2c,0x89,0x64,0xbc,0x04,0xf4,0x54,0xd5,0x67,0x41]

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
