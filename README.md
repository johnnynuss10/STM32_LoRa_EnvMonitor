# STM32_LoRa_EnvMonitor

# Introduction
The purpose of this project is to implement a system to monitor remote environmental conditions. The system consists of mutliple remote sensor nodes, a gateway, and a cloud service.

# Purpose 
In this project, the environmental monitoring nodes are implemented with a battery powered STM32F103C8T6 low power MCU, connecting over I2C to a TI HDC3022 sensor to collect temperature and humidity data. This data is relayed through UART to a LoRa module which transmits environmental data from nodes to a central gateway.  

# Features
- Low-power interrupt driven firmware.
- Sensor nodes battery life >1 yr.
- Optimized BoM for high volume production.
- 

# Pictures
![Sensor Node Schematic](Hardware/EndpointSchematic.png)

# Conclusion
This project consists an environmental monitoring system which transmits from sensor nodes to a central gateway that provides environmental data to a cloud service. The implementation of the gateway and cloud service are not detailed in this repository.

# References

[LoRa Module](https://reyax.com/products/RYLR998) 

[TI HDC3022 Temperature/Humidity sensor](https://www.ti.com/product/HDC3022-Q1)

[STM32F103C8T6 Datasheet](https://www.st.com/resource/en/datasheet/stm32f103cb.pdf)
  
