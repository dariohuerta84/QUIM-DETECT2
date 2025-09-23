# 📊 Predicción de Contaminación del Aire en Florida (2023–2025)

## 📌 Introducción  
El presente trabajo busca **analizar y predecir la concentración diaria de PM10** en Florida utilizando datos de los años **2023, 2024 y 2025**.  
Se aplicaron técnicas de **aprendizaje automático supervisado** para evaluar qué tan bien un modelo podía anticipar los valores de 2025 basándose en los años previos.  

---

## 🛠️ Herramientas y Librerías Utilizadas  
- **Python** como lenguaje principal.  
- **Pandas & NumPy** → limpieza, manipulación y análisis de datos.  
- **Matplotlib & Seaborn** → visualización exploratoria (KDE, pairplots, histogramas).  
- **Scikit-learn** → modelos y métricas.  
  - `LinearRegression` → modelo predictivo base.  
  - `SimpleImputer` → manejo de valores faltantes.  
  - `get_dummies` → codificación de variables categóricas.  

---

## ⚙️ Metodología  
1. **Unión de datasets (2023, 2024, 2025)**.  
2. **Preprocesamiento de fechas**: se generaron variables como `Year`, `Month`, `Day` y `Days_Since_Start`.  
3. **Separación de variables**:  
   - *Numéricas*: latitud, longitud, días, códigos, etc.  
   - *Categóricas*: fuente, estado, condado, etc. (transformadas con *one-hot encoding*).  
4. **Imputación de valores faltantes** mediante la mediana.  
5. **Descartamos columnas como `Daily AQI Value` y `Daily Obs Count`**.  
   - Estas columnas estaban **fuertemente relacionadas con el target (`Daily Mean PM10 Concentration`)**.  
   - En Machine Learning, incluirlas generaría *data leakage*, ya que el modelo tendría información demasiado directa sobre lo que queremos predecir.  
6. **División de datos**:  
   - Entrenamiento → 2023 y 2024.  
   - Prueba → 2025.  
7. **Entrenamiento de modelo de Regresión Lineal**.  
8. **Evaluación con métricas**:  
   - MAE (Error Medio Absoluto)  
   - RMSE (Raíz del Error Cuadrático Medio)  
   - R² (Coeficiente de Determinación)  

---

## 📈 Resultados  
El modelo logró realizar predicciones sobre los datos de 2025.  
Sin embargo, el valor de **R² fue bajo**, lo que indica que la regresión lineal **no explica completamente la variabilidad de los datos**.

**Métricas de evaluación (2025):**
- **MAE**: qué tan lejos estuvo, en promedio, de los valores reales.  
- **RMSE**: penaliza más los errores grandes.  
- **R²**: porcentaje de variabilidad explicada por el modelo.  

---

## ✅ Ventajas  
- Implementación sencilla y rápida.  
- Modelo base **fácil de interpretar**.  
- Pipeline reproducible y aplicable a otros conjuntos de datos.  

## ⚠️ Desventajas  
- La regresión lineal **no captura relaciones no lineales**.  
- El *one-hot encoding* genera demasiadas columnas cuando hay muchas categorías.  
- No se incluyeron factores externos importantes (clima, estaciones, tráfico).  
- Resultados poco satisfactorios en métricas de ajuste.  

---

## 🔧 Posibles Mejoras  
- Probar modelos más complejos: **Random Forest, Gradient Boosting, Redes Neuronales**.  
- Incluir variables externas (clima, humedad, viento).  
- Aplicar **regularización (Ridge/Lasso)** para mejorar la generalización.  
- Selección de características y reducción de dimensionalidad.  
- Implementar técnicas específicas para **series temporales** (ARIMA, Prophet, LSTM).  

---

## 📌 Conclusión  
Este proyecto sirvió como **primer acercamiento al modelado predictivo de la calidad del aire**.  
Aunque los resultados no fueron los esperados, el proceso permitió:  
- Explorar y limpiar datos.  
- Construir un pipeline reproducible.  
- Identificar las limitaciones de un modelo lineal.  
- Aprender la importancia de **evitar data leakage**: descartar variables como *Daily AQI Value* y *Daily Obs Count* que estaban demasiado relacionadas con el target.  

El siguiente paso será **probar modelos no lineales y métodos especializados en series de tiempo** para mejorar la capacidad predictiva.  


El siguiente paso será **probar modelos no lineales y métodos especializados en series de tiempo** para mejorar la capacidad pred
