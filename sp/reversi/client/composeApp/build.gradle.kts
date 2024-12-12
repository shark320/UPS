import org.jetbrains.compose.desktop.application.dsl.TargetFormat


plugins {
    alias(libs.plugins.kotlinMultiplatform)
    alias(libs.plugins.jetbrainsCompose)
    alias(libs.plugins.compose.compiler)
    alias(libs.plugins.kotlin.serialization)
}

java {
    toolchain {
        languageVersion.set(JavaLanguageVersion.of(21)) // Or your desired JDK version
    }
}


kotlin {
    jvm("desktop")
    
    sourceSets {
        val desktopMain by getting
        
        commonMain.dependencies {
            implementation(compose.runtime)
            implementation(compose.foundation)
            implementation(compose.material3)
            implementation(compose.ui)
            implementation(compose.components.resources)
            implementation(compose.components.uiToolingPreview)
            implementation(libs.androidx.lifecycle.viewmodel)
            implementation(libs.androidx.lifecycle.runtime.compose)
            implementation(libs.navigation.compose)

            api(libs.koin.core)
            implementation(libs.koin.compose)
            implementation(libs.koin.compose.viewmodel)

            implementation(libs.kotlinx.serialization.json)

            implementation(libs.ktor.network)

            implementation(libs.log4k.api)
            implementation(libs.log4k.api.kotlin)
            runtimeOnly(libs.log4k.core)
        }
        desktopMain.dependencies {
            implementation(compose.desktop.currentOs)
            implementation(libs.kotlinx.coroutines.swing)
        }
    }

}


compose.desktop {
    application {
        mainClass = "com.vpavlov.ups.reversi.client.MainKt"

        nativeDistributions {
            targetFormats(TargetFormat.Dmg, TargetFormat.Msi, TargetFormat.Deb)
            packageName = "reversiUPS"
            packageVersion = "1.0.0"

            afterEvaluate {
                val copyConfig by tasks.registering(Copy::class) {
                    from("$projectDir/src/desktopMain/resources/config") // Adjust the path as needed
                    //into("$buildDir/compose/binaries/main/app/reversiUPS/bin/config")
                    into(layout.buildDirectory.dir("compose/binaries/main/app/reversiUPS/bin/config")) // Specify the desired external directory
                }

//                tasks["package${TargetFormat.Dmg.name}"].dependsOn(copyConfig)
//                tasks["package${TargetFormat.Msi.name}"].dependsOn(copyConfig)
//                tasks["package${TargetFormat.Deb.name}"].dependsOn(copyConfig)
                tasks["createDistributable"].finalizedBy(copyConfig)
            }
        }
    }
}
