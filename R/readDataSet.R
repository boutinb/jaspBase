#' @export
.setDataSet <- function(dataset) {
  .internal[["dataset"]] <- dataset
}

.readDataSetToEndFromR <- function(columns=NULL, columns.as.numeric=NULL, columns.as.ordinal=NULL, columns.as.factor=NULL, all.columns=FALSE, ...) {
  dataset <- .internal[["dataset"]]
  dataset <- .dataSetSubsetColumns(dataset, columns=columns, columns.as.numeric=columns.as.numeric, columns.as.ordinal=columns.as.ordinal, columns.as.factor=columns.as.factor, all.columns=all.columns, ...)
  return(dataset)
}

.readDataSetHeaderFromR <- function(columns=NULL, columns.as.numeric=NULL, columns.as.ordinal=NULL, columns.as.factor=NULL, all.columns=FALSE, ...) {
  dataset <- .readDataSetToEndFromR(columns=columns, columns.as.numeric=columns.as.numeric, columns.as.ordinal=columns.as.ordinal, columns.as.factor=columns.as.factor, all.columns=all.columns, ...)
  dataset <- dataset[NULL, ]
  return(dataset)
}

.dataSetSubsetColumns <- function(dataset, columns=NULL, columns.as.numeric=NULL, columns.as.ordinal=NULL, columns.as.factor=NULL, all.columns=FALSE, ...) {
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
    lapply(columns, function(col) type(as.character(col)))
  )
}

#' @export
.dataSetColumnSpecification <- function() {
  dataset <- .readDataSetHeaderFromR(all.columns = TRUE)
  specification <- list()
  for(i in seq_len(ncol(dataset))) {
    cl <- class(dataset[[i]])[1]
    specification[[i]] <- list(
      name = colnames(dataset)[[i]],
      type = "scale",
      labels = if(cl %in% c("factor", "ordered")) levels(dataset[[i]]) else NULL
    )
  }
  return(specification)
}
