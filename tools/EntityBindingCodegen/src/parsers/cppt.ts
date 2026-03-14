import { EntityParserFunction, EntityProperty } from "../types";

/**
 * Entity Parser for CPPT entity definitions.
 * e.g. [modules:/zaimodifierrole.class].pc_entitytype
 */
export const parse: EntityParserFunction = (data) => {
  if (!("propertyValues" in data) || !Array.isArray(data["propertyValues"])) {
    return undefined;
  }

  let properties: EntityProperty[] = [];
  for (const prop of data.propertyValues) {
    if (
      prop &&
      // nPropertyID
      "nPropertyID" in prop &&
      typeof prop.nPropertyID === "string" &&
      // value.$type
      "value" in prop &&
      typeof prop.value === "object" &&
      prop.value &&
      "$type" in prop.value &&
      typeof prop.value.$type === "string"
    ) {
      properties.push({
        name: prop.nPropertyID,
        type: prop.value.$type,
      });
    }
  }

  return {
    metadata: {
      parser: "cppt",
    },
    properties,
    pins: [],
  };
};
