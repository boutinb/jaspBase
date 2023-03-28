.setDataSet <- function(dataset) {
  .internal[["dataset"]] <- dataset
}

.readDataSetToEndFromR <- function(columns=NULL, columns.as.numeric=NULL, columns.as.ordinal=NULL, columns.as.factor=NULL, all.columns=FALSE, ...) {
  dataset <- .internal[["dataset"]]
  
  if(!all.columns) {
    dataset <- dataset[, unique(c(columns, columns.as.numeric, columns.as.ordinal, columns.as.factor)), drop = FALSE]
  }

  dataset <- .recodeColumns(dataset, columns.as.numeric, as.numeric)
  dataset <- .recodeColumns(dataset, columns.as.ordinal, as.ordered)
  dataset <- .recodeColumns(dataset, columns.as.factor,  as.factor )

  return(dataset)
}

.recodeColumns <- function(dataset, which, type) {
  if(!is.null(which)) {
    dataset[, which] <- .coerceColumnType(dataset[, which, drop = FALSE], type)
  }
  return(dataset)
}
.coerceColumnType <- function(columns, type) {
  as.data.frame(
    lapply(columns, type)
  )
}