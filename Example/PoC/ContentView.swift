//
//  ContentView.swift
//  PoC
//
//  Created by developer on 07.12.2023.
//

import SwiftUI
import YAD

struct ContentView: View {
    
    init() {
        let a = YAD.decrypt([], [], [])
        print(a)
    }
    
    var body: some View {
        VStack {
            Image(systemName: "globe")
                .imageScale(.large)
            Text("Hello, world!")
        }
        .padding()
    }
}

#Preview {
    ContentView()
}
