# Proyecto: Predicción de PM10 en estaciones de monitoreo

## 📌 Descripción
Este proyecto tiene como objetivo **predecir la concentración diaria de PM10** en distintas estaciones de monitoreo utilizando **regresión lineal**.  
Se trabajó con datos de los años **2023, 2024 y 2025**, aplicando técnicas de ingeniería de características y evaluación de modelos.

---

## ⚙️ Metodología

### 1. Lectura y unión de datos
Se cargaron los datasets de 2023, 2024 y 2025, y se unieron en un único `DataFrame` para facilitar el preprocesamiento.

### 2. Procesamiento de fechas
Se extrajeron variables temporales relevantes:
- `year`, `month`, `dayofweek`, `dayofyear`.

### 3. Codificación de estaciones
Se utilizó **OneHotEncoder** para transformar la variable categórica **Site ID** en variables binarias.  
- Se entrenó el encoder con **2023–2024** para evitar fugas de información.  
- Se incluyó la opción `handle_unknown="ignore"` para que nuevas estaciones en 2025 no generaran errores.

### 4. Creación de variables de retraso (lags)
Se generaron variables que representan el historial de contaminación en la misma estación:
- `PM10_lag1`: día anterior.  
- `PM10_lag7`: una semana antes.  
- `PM10_lag30`: un mes antes.  

Estas variables permiten que el modelo tenga **memoria temporal**.

### 5. Representación cíclica de variables de tiempo
Mes, día de la semana y día del año se transformaron en componentes **seno y coseno** para capturar su **naturaleza cíclica**:
- `month_sin`, `month_cos`  
- `dow_sin`, `dow_cos`  
- `doy_sin`, `doy_cos`  

Esto evita que el modelo interprete de forma incorrecta el paso de diciembre (12) a enero (1).

### 6. Definición de variables de entrada
El conjunto de features incluyó:
- Lags de PM10.  
- Variables cíclicas de tiempo.  
- Variables one-hot de las estaciones.  

### 7. Eliminación de valores faltantes
Se eliminaron las primeras filas de cada estación que no podían tener lags (por ejemplo, no existe `PM10_lag7` para los primeros 7 días).

### 8. División de los datos
- **Train:** 2023  
- **Test:** 2024  
- **Future:** 2025  

### 9. Entrenamiento
Se entrenó un **modelo de regresión lineal** con los datos de 2023.

### 10. Evaluación
- **Validación en 2024 (test)**: se midió qué tan bien generaliza el modelo a un año futuro.  
- **Evaluación en 2025 (futuro)**: se compararon predicciones con los datos reales de 2025.

---

## 📊 Resultados

### Evaluación en 2024
```tex
MSE: 289.39
RMSE: 17.01
R²: 0.44
```
### Evaluación en 2025
```text
MSE: 239.74
RMSE: 15.48
R²: 0.38
```

## 📝 Conclusiones

1. **Rendimiento del modelo**:  
   - El modelo de regresión lineal logra capturar parte de la variabilidad del PM10 en los años 2024 y 2025, con un R² moderado (0.44 en 2024 y 0.38 en 2025).  
   - El RMSE indica un error promedio de predicción de ~17 µg/m³ en 2024 y ~15.5 µg/m³ en 2025.

2. **Interpretación de los resultados**:  
   - Aunque el R² disminuye ligeramente en 2025, el modelo sigue siendo capaz de reflejar tendencias generales de PM10.  
   - La reducción del RMSE en 2025 sugiere menor variabilidad en los datos de ese año o que las variables de retraso y cíclicas ayudaron a mejorar la precisión.

3. **Fortalezas**:  
   - Uso de variables de retraso (lags) que permiten incorporar memoria temporal.  
   - Representación cíclica de tiempo que evita problemas con variables como mes o día de la semana.  
   - Manejo de nuevas estaciones en 2025 gracias a `handle_unknown="ignore"` en OneHotEncoder.

4. **Limitaciones y mejoras futuras**:  
   - El modelo lineal puede no capturar relaciones no lineales complejas entre las variables.  
   - Se podrían explorar modelos más avanzados (Random Forest, Gradient Boosting o redes neuronales) para mejorar R² y reducir el RMSE.  
   - Incorporar variables meteorológicas y contaminantes adicionales podría aumentar la capacidad predictiva.

