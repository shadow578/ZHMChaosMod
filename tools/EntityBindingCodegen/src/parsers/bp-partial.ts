import { EntityParserFunction } from "../types";
import { parseEntity } from "./bp";

/**
 * Entity Parser for partial blueprint entity definitions
 * (as copied from GlacierKit entity template editor).
 */
export const parse: EntityParserFunction = (data) => {
  if (!("parent" in data) || !("factory" in data)) {
    return undefined;
  }

  const parsed = parseEntity(data);
  if (!parsed) {
    return undefined;
  }

  return {
    metadata: {
      name: parsed.name,
      parser: "bp_partial",
    },
    properties: parsed.properties,
    pins: parsed.pins,
    exposedInterfaces: parsed.exposedInterfaces,
  };
};
