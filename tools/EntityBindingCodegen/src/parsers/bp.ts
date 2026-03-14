import { EntityParserFunction, EntityPin, EntityProperty } from "../types";

export function parseEntity(
  data: object,
  lookupEntity: (eid: string) => object | undefined = () => undefined,
):
  | {
      name?: string;
      properties: EntityProperty[];
      pins: EntityPin[];
    }
  | undefined {
  let name = undefined;
  let properties: EntityProperty[] = [];
  let pins: EntityPin[] = [];

  if ("name" in data && typeof data.name === "string") {
    name = data.name;
  }

  if (
    "properties" in data &&
    typeof data.properties === "object" &&
    data.properties
  ) {
    for (const [name, prop] of Object.entries(data.properties)) {
      if (
        prop &&
        // type
        "type" in prop &&
        typeof prop.type === "string"
      ) {
        properties.push({
          name,
          type: prop.type,
        });
      }
    }
  }

  if (
    "propertyAliases" in data &&
    typeof data.propertyAliases === "object" &&
    data.propertyAliases
  ) {
    for (const [name, aliases] of Object.entries(data.propertyAliases)) {
      let type = undefined;

      for (const alias of aliases) {
        if (
          "originalEntity" in alias &&
          typeof alias.originalEntity === "string" &&
          "originalProperty" in alias &&
          typeof alias.originalProperty === "string"
        ) {
          const originalEntity = lookupEntity(alias.originalEntity);
          if (
            originalEntity &&
            "properties" in originalEntity &&
            typeof originalEntity.properties === "object" &&
            originalEntity.properties
          ) {
            const originalProp =
              // @ts-expect-error - wonky typing
              originalEntity.properties[alias.originalProperty];

            if (
              originalProp &&
              typeof originalProp === "object" &&
              "type" in originalProp &&
              typeof originalProp.type === "string"
            ) {
              type = originalProp.type;
              break;
            }
          }
        }
      }

      properties.push({
        name,
        type,
      });
    }
  }

  if (
    "inputCopying" in data &&
    typeof data.inputCopying === "object" &&
    data.inputCopying
  ) {
    for (const [name, _] of Object.entries(data.inputCopying)) {
      pins.push({
        name,
        direction: "input",
      });
    }
  }

  if (
    "outputCopying" in data &&
    typeof data.outputCopying === "object" &&
    data.outputCopying
  ) {
    for (const [name, _] of Object.entries(data.outputCopying)) {
      pins.push({
        name,
        direction: "output",
      });
    }
  }

  return {
    name,
    properties,
    pins,
  };
}

/**
 * Entity Parser for partial blueprint entity definitions
 * (as copied from GlacierKit entity template editor).
 */
export const parse: EntityParserFunction = (data) => {
  if (
    !("rootEntity" in data && typeof data.rootEntity === "string") ||
    !("entities" in data && typeof data.entities === "object" && data.entities)
  ) {
    return undefined;
  }

  const lookupEntity = (eid: string) => {
    // @ts-expect-error - wonky typing
    if (!(eid in data.entities)) {
      return undefined;
    }

    // @ts-expect-error - wonky typing
    return data.entities[eid];
  };

  const parsed = parseEntity(lookupEntity(data.rootEntity), lookupEntity);
  if (!parsed) {
    return undefined;
  }

  let hash = undefined;
  if ("tempHash" in data && typeof data.tempHash === "string") {
    hash = data.tempHash;
  }

  return {
    metadata: {
      name: parsed.name,
      hash,
      parser: "bp",
    },
    properties: parsed.properties,
    pins: parsed.pins,
  };
};
