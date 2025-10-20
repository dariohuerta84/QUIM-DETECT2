

# 🧩 Análisis Tecnológico 

| **Subfunción** | **Boceto 1 – Cámara multiespectral** | **Boceto 2 – Escáner UV con pantalla** | **Boceto 3 – Sistema AS7343 con caja giratoria** |
|----------------|--------------------------------------|-----------------------------------------|--------------------------------------------------|
| **Rotar fresa / toma de muestra** | No aplica (captura fija) | Sin movimiento | Motor paso a paso con brazo rotable |
| **Detección** | Cámara multiespectral de alta gama | LED UV + fotodiodo TEMP6000 | Sensor AS7343 visible/NIR |
| **Control y adquisición** | Raspberry Pi | ESP32 | Arduino Nano 33 IoT |
| **Procesamiento de datos** | Software propietario | Pantalla LCD con lógica simple | Google Colab (Python) |
| **Energía** | Batería Li-ion 7.4V | Batería recargable 5V | Batería 18650 |


---

## ⚖️ Criterios de Evaluación Tecnológica

| **Criterio** | **Peso** |
|---------------|----------|
| Precisión de medición | 0.30 |
| Costo total | 0.25 |
| Complejidad técnica (menor = mejor) | 0.15 |
| Mantenimiento y calibración | 0.15 |
| Robustez y fiabilidad | 0.10 |
| Consumo energético | 0.05 |
| **Total** | **1.00** |

---

### 📊 Ponderación de los 3 Bocetos

| **Criterio** | **Peso** | **Boceto 1** | **Boceto 2** | **Boceto 3** | **P*B1** | **P*B2** | **P*B3** |
|---------------|----------|--------------|--------------|--------------|-----------|-----------|-----------|
| Precisión | 0.30 | 5 | 3 | 4 | 1.50 | 0.90 | 1.20 |
| Costo | 0.25 | 1 | 4 | 5 | 0.25 | 1.00 | 1.25 |
| Complejidad | 0.15 | 2 | 3 | 4 | 0.30 | 0.45 | 0.60 |
| Mantenimiento | 0.15 | 3 | 4 | 4 | 0.45 | 0.60 | 0.60 |
| Robustez | 0.10 | 3 | 3 | 4 | 0.30 | 0.30 | 0.40 |
| Consumo | 0.05 | 2 | 3 | 4 | 0.10 | 0.15 | 0.20 |
| **Total ponderado** | — | — | — | — | **2.90** | **3.40** | **4.25** |

---

# 💰 Análisis Económico

Este análisis compara los **costos estimados** de los tres bocetos desarrollados durante la fase de diseño.  
Los precios están expresados en **soles peruanos (S/.)**, basados en componentes disponibles localmente.

| *Boceto* | *Material / Tipo* | *Costo (S/.)* |
|-------------|---------------------|-----------------|
| **Boceto 1 – Cámara multiespectral** | Sensor óptico de alta gama | 3040 |
|  | Electrónica + carcasa plástica | 240 |
|  | Batería Li-ion 7.4V | 95 |
|  | Carcasa de fibra de vidrio | 115 |
|  | Lentes ópticos | 380 |
| **➡ TOTAL BOCETO 1** | | **3,870** |
| | | |
| **Boceto 2 – Escáner UV con pantalla** | Placa ESP32 (PCB) | 45 |
|  | LED UV | 15 |
|  | Fotodiodo TEMP6000 | 30 |
|  | Pantalla LCD táctil | 140 |
|  | Batería recargable 5V | 40 |
|  | Botones + interruptor | 20 |
| **➡ TOTAL BOCETO 2** | | **290** |
| | | |
| **Boceto 3 – Sistema AS7343 con caja giratoria** | Arduino Nano 33 IoT | 30 |
|  | Sensor AS7343 | 87 |
|  | LED UV | 15 |
|  | Protoboard PCB | 12 |
|  | Batería 18650 | 30 |
|  | Motor paso a paso | 40 |
| **➡ TOTAL BOCETO 3** | | **214** |

---

## 🧮 Comparación Global

| **Boceto** | **Costo total (S/.)** | **Puntaje tecnológico** | **Conclusión** |
|-------------|------------------------|--------------------------|----------------|
| **Boceto 1** | 3,870 | 2.90 | Demasiado costoso; poca viabilidad económica. |
| **Boceto 2** | 290 | 3.40 | Económico pero de baja precisión espectral. |
| **🟢 Boceto 3** | **214** | **4.25** | **Ganador: equilibrio óptimo entre costo, precisión y simplicidad.** |

---

## 🧩 Conclusión Final

Tras los análisis **tecnológico y económico**, el **Boceto 3 – Sistema AS7343 con caja giratoria automatizada** resulta ser la opción **más viable y eficiente** para el proyecto **QUIM-DETECT2**.

**Ventajas clave:**
- Alta **relación costo/beneficio**.  
- **Automatización controlada por Arduino** y fácil integración con Colab.  
- **Sensor AS7343** con amplio rango espectral visible/NIR.  
- Bajo consumo energético y mantenimiento simple.

**Resultado final:**
> ✅ **Boceto 3 es el diseño seleccionado para la etapa de prototipo funcional.**

---




