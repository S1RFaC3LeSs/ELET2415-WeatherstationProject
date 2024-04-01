<template>
    <v-container>
      <v-row
        ><v-col align="center">
          <v-card
            class="mb-5"
            style="max-width: 500px"
            title="DHT22 Sensor Component"
          >
            <v-card-item>
              <span class="text-onPrimaryContainer text-h4">
                <v-img
                  :width="316"
                  aspect-ratio="1/1"
                  cover
                  src="@/assets/pic1.jpg"></v-img>
                <a
                  href="https://components101.com/sensors/dht22-pinout-specs-datasheet"
                  target="_blank"
                  >Click Me For More Info</a
                >
              </span>
            </v-card-item>
          </v-card>
        </v-col>
      </v-row>
      <v-row
        ><v-col align="center">
          <v-card
            class="mb-5"
            style="max-width: 500px"
            title="BPM Sensor Component"
          >
            <v-card-item>
              <span class="text-onPrimaryContainer text-h4">
                <v-img
                  :width="200"
                  aspect-ratio="1/1"
                  cover
                  src="@/assets/pic2.jpg"
                ></v-img>
                <a
                  href="https://components101.com/sensors/gy-bmp280-module"
                  target="_blank"
                  >Click Me For More Info</a
                >
              </span>
            </v-card-item>
          </v-card>
        </v-col></v-row
      >
      <v-row
        ><v-col align="center">
          <v-card
            class="mb-5"
            style="max-width: 500px"
            title="Soil Moisture Sensor Component"
          >
            <v-card-item>
              <span class="text-onPrimaryContainer text-h4">
                <v-img
                  :width="200"
                  aspect-ratio="1/1"
                  cover
                  src="@/assets/pic3.jpg"
                ></v-img>
                <a
                  href="https://media.digikey.com/pdf/data%20sheets/dfrobot%20pdfs/sen0193_web.pdf"
                  target="_blank"
                  >Click Me For More Info</a
                >
              </span>
            </v-card-item>
          </v-card>
        </v-col></v-row
      >
      <v-row
        ><v-col align="center">
          <v-card
            class="mb-5"
            style="max-width: 500px"
            title="Data Processing Component"
          >
            <v-card-item>
              <span class="text-onPrimaryContainer text-h4">
                <v-img
                  :width="200"
                  aspect-ratio="1/1"
                  cover
                  src="@/assets/pic4.jpg"
                ></v-img>
                <a
                  href="https://docs.espressif.com/projects/esp-idf/en/stable/esp32/hw-reference/esp32/get-started-devkitc.html"
                  target="_blank"
                  >Click Me For More Info</a
                >
              </span>
            </v-card-item>
          </v-card>
        </v-col></v-row
      >
      <v-row
        ><v-col align="center">
          <v-card class="mb-5" style="max-width: 500px" title="Display Component">
            <v-card-item>
              <span class="text-onPrimaryContainer text-h4">
                <v-img
                  :width="200"
                  aspect-ratio="1/1"
                  cover
                  src="@/assets/tft.jpg"
                ></v-img>
                <a
                  href="http://www.lcdwiki.com/2.8inch_SPI_Module_ILI9341_SKU:MSP2807"
                  target="_blank"
                  >Click Me For More Info</a
                >
              </span>
            </v-card-item>
          </v-card>
        </v-col></v-row
      >
    </v-container>
  </template>
  
  <script setup>
  /** JAVASCRIPT HERE */
  
  import { useMqttStore } from "@/store/mqttStore"; // Import Mqtt Store
  import { storeToRefs } from "pinia";
  
  // IMPORTS
  import {
    ref,
    reactive,
    watch,
    onMounted,
    onBeforeUnmount,
    computed,
  } from "vue";
  import { useRoute, useRouter } from "vue-router";
  
  // VARIABLES
  const router = useRouter();
  const route = useRoute();
  
  const Mqtt = useMqttStore();
  const { payload, payloadTopic } = storeToRefs(Mqtt);
  
  // FUNCTIONS
  onMounted(() => {
    // THIS FUNCTION IS CALLED AFTER THIS COMPONENT HAS BEEN MOUNTED
    Mqtt.connect(); // Connect to Broker located on the backend
    setTimeout(() => {
      // Subscribe to each topic
      Mqtt.subscribe("620146473");
      Mqtt.subscribe("620146473_sub");
    }, 3000);
  });
  
  onBeforeUnmount(() => {
    // THIS FUNCTION IS CALLED RIGHT BEFORE THIS COMPONENT IS UNMOUNTED
    Mqtt.unsubcribeAll();
  });
  </script>
  
  <style scoped>
  /** CSS STYLE HERE */
  </style>
  