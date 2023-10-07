//
//  YDPPUtil.swift
//  YAD
//
//  Created by Developer on 30.09.2023.
//

import Foundation
import CryptoSwift

public class YAD {
    
    fileprivate init() {}
    
    public static func decrypt(_ data: [UInt8], _ dataId: [UInt8], _ keyBasis: [UInt8], _ iv: [UInt8] = YDConstants.aesIV) -> [UInt8] {
        let key = processKeyBasis(keyBasis, dataId)
        if (key.isEmpty) {
            return []
        }
        let decrypted = aesCtr128Decrypt(buffer: data, key: key, iv: iv)
        return decrypted
    }
    
    static func processKeyBasis(_ keyBasis: [UInt8], _ dataId: [UInt8]) -> [UInt8] {
        let keyPtr = UnsafeMutablePointer<UInt8>.allocate(capacity: keyBasis.count)
        keyPtr.initialize(repeating: 0, count: keyBasis.count)
        keyPtr.assign(from: keyBasis, count: keyBasis.count)
        let dataIdPtr = UnsafeMutablePointer<UInt8>.allocate(capacity: dataId.count)
        dataIdPtr.initialize(repeating: 0, count: dataId.count)
        dataIdPtr.assign(from: dataId, count: dataId.count)
        let resPtr = UnsafeMutablePointer<UInt8>.allocate(capacity: 16)
        let length = unwrapKey(keyPtr, dataIdPtr, resPtr)
        if (length != 16) {
            #if DEBUG
            print("Expected key length 16, but was " + String(length))
            #endif
            return []
        }
        let buffer = UnsafeMutableBufferPointer<UInt8>(start: resPtr, count: Int(length))
        let keyData = Data(buffer: buffer)
        let keyBytes = [UInt8].init(keyData)
        resPtr.deallocate()
        keyPtr.deallocate()
        dataIdPtr.deallocate()
        #if DEBUG
        print("key -> ", keyBytes)
        #endif
        return keyBytes
    }
    
    fileprivate static func aesCtr128Decrypt(buffer: [UInt8], key: [UInt8], iv: [UInt8]) -> [UInt8] {
        if (key.count != 16 || iv.count != 16) {
            #if DEBUG
            print("Incorrect key or iv size: key - " + String(key.count) + "; iv - " + String(iv.count))
            #endif
            return []
        }
        do {
            let aesEng = try AES(key: key, blockMode: CTR(iv: iv))
            let decrypted = try aesEng.decrypt(buffer)
            return decrypted
        } catch {
            #if DEBUG
            print(error)
            #endif
        }
        return []
    }
}
