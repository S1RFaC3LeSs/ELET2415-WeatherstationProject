<template>
  
  <VContainer class="fill-height ">
    <VResponsive class="align-center text-center fill-height">
      <VImg height="350" class="mb-5" src="@/assets/ice cube.jpg" />

      <div class="text-body-2 font-weight-light mb-n1">Lets find out the weather</div>

      <h1 class="text-h2 font-weight-bold">Go to the next page</h1>
      <div class="text-body-1 font-weight-light mt-1"></div>
 

      <div class="py-14" />

      <VRow class="d-flex align-center justify-center">
        <VCol cols="auto">
          <VBtn
            color="primary"
            href="https://www.tiktok.com/@joethehoboe/video/7285751609055939886"
            min-width="164"
            rel="noopener noreferrer"
            target="_blank"
            variant="text"
          >
            <VIcon icon="" size="large" start />

            How College Feels
          </VBtn>
        </VCol>

        <VCol cols="auto">
          <VBtn
            color="primary"
            href="https://www.google.com/search?q=weather+in+jamaica&rlz=1C1CHBF_enJM868JM868&oq=weather+in+jama&gs_lcrp=EgZjaHJvbWUqCggAEAAYsQMYgAQyCggAEAAYsQMYgAQyBwgBEAAYgAQyBggCEEUYOTIHCAMQABiABDIHCAQQABiABDIHCAUQABiABDIHCAYQABiABDIHCAcQABiABDIHCAgQABiABDIHCAkQABiABKgCALACAA&sourceid=chrome&ie=UTF-8"
            min-width="164"
            rel="noopener noreferrer"
            target="_blank"
            variant="text"
          >
            <VIcon icon="" size="large" start />

            Google Reference
          </VBtn>
        </VCol>

        <VCol cols="auto">
          <VBtn
            color="primary"
            href="https://www.youtube.com/@weatherjamaica/videos"
            min-width="164"
            rel="noopener noreferrer"
            target="_blank"
            variant="text"
          >
            <VIcon icon="" size="large" start />

            Weather Channel
          </VBtn>
        </VCol>
      </VRow>
    </VResponsive>
  </VContainer>

</template>

<script setup>
  import { storeToRefs } from 'pinia';
  import { useMqttStore } from '@/store/mqttStore';
  import { ref,reactive, watch, onMounted, onBeforeUnmount } from 'vue';


// VARIABLES


// FUNCTIONS
onMounted(()=>{
   
});

onBeforeUnmount(()=>{ 
  
});
</script>
